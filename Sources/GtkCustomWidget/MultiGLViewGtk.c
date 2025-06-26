/*
 * MultiGLViewGtk.c
 * 
 * Copyright 2025 Guillaume Saumure <gsaumure@cgocable.ca>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>

#include <epoxy/gl.h>

#include "SimpleGLViewGtk.h"
#include "MultiGLViewGtk.h"

typedef struct _View
{
    GLuint fbo;
    //GLuint depth_stencil_buffer;
    GLenum status;
    int width, height;
    GdkGLTextureBuilder *builder;
    GLuint TextureID;
    GdkTexture* gl_texture;
   // GdkTexture* dmabuf_texture;
} View;

typedef struct _MultiGLViewPrivate MultiGLViewPrivate;

struct _MultiGLViewPrivate
{
	GdkDisplay* display;
	GdkSurface* surface; 
	GdkGLContext* context;
	GError* error;
	
	int required_gl_version;
	GdkGLAPI allowed_apis;
	
	//gboolean have_depth_buffers;
	//gboolean have_stencil_buffers;
	gboolean needs_resize;
	gboolean auto_render;
	gboolean needs_render;
	//gboolean have_buffers;
	gboolean maximized_mode;
	
	View views[5];
	RenderCallback render_scene;
    void* userdata;	
	GtkWidget* main_paned;
	GtkWidget* top_paned;
	GtkWidget* bottom_paned;
	GtkWidget* small_view_overlays[4];
	GtkWidget* maximize_view_overlay;
	GtkWidget* small_views[4];
	GtkWidget* maximize_view;
	
		
};

G_DEFINE_TYPE_WITH_PRIVATE(MultiGLView, multi_gl_view, GTK_TYPE_BOX)


static GdkGLContext* multi_gl_view_create_context(MultiGLView*);
static void multi_gl_view_realize(GtkWidget *widget);
static void multi_gl_view_unrealize(GtkWidget *widget);
static void multi_gl_view_size_allocate(GtkWidget *widget, int width, int height, int baseline);
static void multi_gl_view_snapshot(GtkWidget* widget, GtkSnapshot* snapshot);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// An enum is created with 2 contants and an array
// of only one element. It's being done that way for
// future expansion.

enum
{
  CREATE_CONTEXT,
  LAST_SIGNAL
};

static guint multi_gl_view_signals[LAST_SIGNAL] = {0};

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// This function is adapted from the official gtkglarea.c

static gboolean multi_gl_view_create_context_accumulator(GSignalInvocationHint *ihint,
                            GValue *return_accu, const GValue *handler_return, gpointer data)
{
	g_value_copy(handler_return, return_accu);
	/* stop after the first handler returning a valid object */
	return g_value_get_object(handler_return) == NULL;
}

static void multi_gl_view_class_init(MultiGLViewClass* klass)
{
    //GObjectClass* object_class = G_OBJECT_CLASS(klass);
    
    klass->create_context = multi_gl_view_create_context;
    
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    
    widget_class->realize = multi_gl_view_realize;
    widget_class->unrealize = multi_gl_view_unrealize;
    widget_class->size_allocate = multi_gl_view_size_allocate;
    widget_class->snapshot = multi_gl_view_snapshot;
    

  multi_gl_view_signals[CREATE_CONTEXT] =
    g_signal_new ("create-context",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (MultiGLViewClass, create_context),
                  multi_gl_view_create_context_accumulator, NULL,
                  g_cclosure_marshal_generic,
                  GDK_TYPE_GL_CONTEXT, 0);
}

static GdkGLContext* multi_gl_view_create_context(MultiGLView* self) 
{
   // MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
    GError* error = NULL;
    GdkGLContext* context = NULL;

    GdkDisplay* display = gdk_display_get_default();
    
    if (!display) 
    {
        g_warning("No default GdkDisplay available");
        return NULL;
    }
    
    if (!gdk_display_prepare_gl(display, &error)) 
    {
        g_warning("Display does NOT support GL: %s", error ? error->message : "Unknown");
        multi_gl_view_set_error(self, error);
        g_clear_error(&error);
        return NULL;
    }

    context = gdk_display_create_gl_context(display, &error);
    
    if (context == NULL) 
    {
        g_warning("gdk_display_create_gl_context failed: %s", error ? error->message : "No error message");
        multi_gl_view_set_error(self, error);
        g_clear_error(&error);
        return NULL;
    }

    if (!gdk_gl_context_realize(context, &error)) 
    {
		g_warning("gdk_gl_context_realize failed: %s", error ? error->message : "No error message");
		multi_gl_view_set_error(self, error);
        g_clear_object(&context);
        g_clear_error(&error);
        return NULL;
    }

    return context;
}

static void multi_gl_view_realize(GtkWidget *widget)
{
	MultiGLView* self = MULTI_GL_VIEW(widget);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	GTK_WIDGET_CLASS(multi_gl_view_parent_class)->realize(widget);
	
	if (private->error)
	{
		return;
	}

	g_signal_emit(self, multi_gl_view_signals[CREATE_CONTEXT], 0, &private->context);
	
	if (private->context == NULL)
	{
		return;
	}
	
	/* In case the signal failed, but did not set an error */
	if (private->context == NULL && private->error == NULL)
	{
		g_set_error_literal(&private->error, GDK_GL_ERROR,
                         GDK_GL_ERROR_NOT_AVAILABLE,
                         "OpenGL context creation failed");
        
        return;
	}
	
	multi_gl_view_make_current(self);
	
	
    for (int ViewID = 0; ViewID < 5; ViewID++)
    {
		private->views[ViewID].builder = gdk_gl_texture_builder_new();
		gdk_gl_texture_builder_set_context(private->views[ViewID].builder, GDK_GL_CONTEXT(private->context));
		
		if (gdk_gl_context_get_api (private->context) == GDK_GL_API_GLES)
			gdk_gl_texture_builder_set_format (private->views[ViewID].builder, GDK_MEMORY_R8G8B8A8_PREMULTIPLIED);
		else
			gdk_gl_texture_builder_set_format (private->views[ViewID].builder, GDK_MEMORY_B8G8R8A8_PREMULTIPLIED);

	}
	
	private->needs_resize = TRUE;
}

static void multi_gl_view_unrealize(GtkWidget *widget)
{
	MultiGLView* self = MULTI_GL_VIEW(widget);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	
	
	
	GTK_WIDGET_CLASS(multi_gl_view_parent_class)->unrealize(widget);
}

static void multi_gl_view_release_gl_texture(gpointer data)
{
	View* view = data;
    view->gl_texture = NULL;
}

static void multi_gl_view_ensure_view_buffer(View* view)
{

	if (view->fbo == 0)
	{
		glGenFramebuffers (1, &view->fbo);
	}
	
} 

static void multi_gl_view_ensure_view_texture(View* view)
{
	if (view->TextureID == 0)
	{
		glGenTextures(1, &view->TextureID);
		gdk_gl_texture_builder_set_id(view->builder, view->TextureID);
	}
	else
	{
		glDeleteTextures(1, &view->TextureID);
		glGenTextures(1, &view->TextureID);
		gdk_gl_texture_builder_set_id(view->builder, view->TextureID);
	}
}

static void multi_gl_view_allocate_view_texture(MultiGLView* self, View* view)
{
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);

	if (gdk_gl_texture_builder_get_width(view->builder) != view->width || 
		gdk_gl_texture_builder_get_height(view->builder) != view->height)
	{
		glBindTexture (GL_TEXTURE_2D, gdk_gl_texture_builder_get_id(view->builder));
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		if (gdk_gl_context_get_api(private->context) == GDK_GL_API_GLES)
		{
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, view->width, view->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}
		else
		{
			glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, view->width, view->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		}
		
		gdk_gl_texture_builder_set_width(view->builder, view->width);
		gdk_gl_texture_builder_set_height(view->builder, view->height);
	}
}

static void multi_gl_view_attach_view_buffer(View* view)
{
	glBindFramebuffer(GL_FRAMEBUFFER, view->fbo);
		
	if (view->TextureID > 0)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gdk_gl_texture_builder_get_id(view->builder), 0);
	}

	view->status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
}




static void multi_gl_view_resize_view(MultiGLView* self, int ViewID, int width, int height) 
{
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
  
	View* view = &private->views[ViewID];
	
    if (view->width == width && view->height == height) 
    {
		multi_gl_view_attach_view_buffer(view);
		return;
	}
	else
	{
		view->width = width;
		view->height = height;
	}
	
	multi_gl_view_ensure_view_buffer(view);
	multi_gl_view_ensure_view_texture(view);
	multi_gl_view_allocate_view_texture(self, view);
	multi_gl_view_attach_view_buffer(view);
}

static void multi_gl_view_snapshot(GtkWidget* widget, GtkSnapshot* snapshot) 
{
	MultiGLView* self = MULTI_GL_VIEW(widget);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);

	if (private->error) 
	{
		for (int i = 0; i < 5; i++) 
		{
			if (i < 4)
			{
				simple_gl_view_set_error(SIMPLE_GL_VIEW(private->small_views[i]), private->error);
			}
			else
			{
				simple_gl_view_set_error(SIMPLE_GL_VIEW(private->maximize_view), private->error);
			}
		}
		
		private->needs_render = FALSE;
		
		// Delegate to parent (GtkBox) to snapshot children
		GTK_WIDGET_CLASS(multi_gl_view_parent_class)->snapshot(widget, snapshot);
		
		return;
	}
	
	

	
	if (private->needs_render && private->render_scene && private->context) 
	{
		multi_gl_view_make_current(self);
		
		if (!private->maximized_mode)
		{
			for (int i = 0; i < 4; i++) 
			{
				int s = gtk_widget_get_scale_factor(private->small_views[i]);
				int w = gtk_widget_get_width(private->small_views[i]) * s;
				int h = gtk_widget_get_height(private->small_views[i]) * s;
				
				if (w > 0 && h > 0) 
				{
					multi_gl_view_resize_view(self, i, w, h); 
				}
				else
				{
					return;
				}
			
			}
		}
		else
		{
			int s = gtk_widget_get_scale_factor(private->maximize_view);
			int w = gtk_widget_get_width(private->maximize_view) * s;
			int h = gtk_widget_get_height(private->maximize_view) * s;
			
			if (w > 0 && h > 0) 
			{
				multi_gl_view_resize_view(self, 4, w, h); 
			}
			else
			{
				return;
			}
		}
		
		if (!private->maximized_mode)
		{
			for (int i = 0; i < 4; i++) 
			{
				if (private->views[i].status != GL_FRAMEBUFFER_COMPLETE)
				{
					g_warning("Framebuffer setup not complete (%d)", private->views[i].status);
				}
			}
		}
		else
		{
			if (private->views[4].status != GL_FRAMEBUFFER_COMPLETE)
			{
				g_warning("Framebuffer setup not complete (%d)", private->views[4].status);
			}
		}
		
		if (!private->maximized_mode)
		{
			for (int i = 0; i < 4; i++) 
			{
				private->render_scene(self, i, private->views[i].fbo, private->views[i].width, private->views[i].height, private->userdata);
			}
		}
		else
		{
			private->render_scene(self, 4, private->views[4].fbo, private->views[4].width, private->views[4].height, private->userdata);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glFinish();
		
		//GdkDmabuf dmabuf;

		gpointer sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		
		if (!private->maximized_mode)
		{
			for (int i = 0; i < 4; i++) 
			{
				gdk_gl_texture_builder_set_sync(GDK_GL_TEXTURE_BUILDER(private->views[i].builder), sync);
				
				private->views[i].gl_texture = gdk_gl_texture_builder_build(GDK_GL_TEXTURE_BUILDER(private->views[i].builder),
															  multi_gl_view_release_gl_texture, (void*) &private->views[i]);
				
				simple_gl_view_set_texture(SIMPLE_GL_VIEW(private->small_views[i]), private->views[i].gl_texture);
			}
		}
		else
		{
			gdk_gl_texture_builder_set_sync(GDK_GL_TEXTURE_BUILDER(private->views[4].builder), sync);
				
			private->views[4].gl_texture = gdk_gl_texture_builder_build(private->views[4].builder,
															  multi_gl_view_release_gl_texture, (void*) &private->views[4]);
				
			simple_gl_view_set_texture(SIMPLE_GL_VIEW(private->maximize_view), private->views[4].gl_texture);
		}

		
		if (sync)
		{
			glDeleteSync(sync);
			
			if (!private->maximized_mode)
			{
				for (int i = 0; i < 4; i++) 
				{
					gdk_gl_texture_builder_set_sync(private->views[i].builder, NULL);
				}
			}
			else
			{
				gdk_gl_texture_builder_set_sync(private->views[4].builder, NULL);
			}
		}
		
		private->needs_render = FALSE;
		
		// Delegate to parent (GtkBox) to snapshot children
		GTK_WIDGET_CLASS(multi_gl_view_parent_class)->snapshot(widget, snapshot);
	}

}

static void multi_gl_view_size_allocate(GtkWidget *widget, int width, int height, int baseline)
{
	MultiGLView* self = MULTI_GL_VIEW(widget);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	GTK_WIDGET_CLASS(multi_gl_view_parent_class)->size_allocate(widget, width, height, baseline);
	
	if (!gtk_widget_get_realized(widget) || private->error)
	{
		return;
	}

	if (!private->maximized_mode)
	{
		for (int i = 0; i < 4; i++) 
		{
			int s = gtk_widget_get_scale_factor(private->small_views[i]);
            int w = gtk_widget_get_width(private->small_views[i]) * s;
            int h = gtk_widget_get_height(private->small_views[i]) * s;
            if (w > 0 && h > 0) multi_gl_view_resize_view(self, i, w, h);
        }
	}
	else
	{
		int s = gtk_widget_get_scale_factor(private->maximize_view);
		int w = gtk_widget_get_width(private->maximize_view) * s;
		int h = gtk_widget_get_height(private->maximize_view) * s;
		if (w > 0 && h > 0) multi_gl_view_resize_view(self, 4, w, h);
	}
	
	multi_gl_view_queue_render(self);
	
}

static void multi_gl_view_init(MultiGLView* self)
{
    MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
    
	private->context = NULL;
	private->error = NULL;
	//private->have_buffers = FALSE;
	private->required_gl_version = 0;
	private->allowed_apis = GDK_GL_API_GL | GDK_GL_API_GLES;
	
	private->needs_resize = TRUE;
	private->auto_render = TRUE;
	private->needs_render = TRUE;

	private->main_paned = NULL;
	private->top_paned = NULL;
	private->bottom_paned = NULL;
	
	private->small_view_overlays[0] = NULL;
	private->small_view_overlays[1] = NULL;
	private->small_view_overlays[2] = NULL;
	private->small_view_overlays[3] = NULL;
	private->maximize_view_overlay = NULL;	
	
	private->small_views[0] = NULL;
	private->small_views[1] = NULL;
	private->small_views[2] = NULL;
	private->small_views[3] = NULL;
	private->maximize_view = NULL;
    
    private->maximized_mode = FALSE;
    
    for (int ViewID = 0; ViewID < 5; ViewID++)
    {
		private->views[ViewID].fbo = 0;
		private->views[ViewID].width = 0;
		private->views[ViewID].height = 0;
		private->views[ViewID].builder = NULL;
		private->views[ViewID].TextureID = 0;
		private->views[ViewID].gl_texture = NULL;
		//private->views[ViewID].dmabuf_texture = NULL;
	}
	
    private->render_scene = NULL;
    private->userdata = NULL;
    
    
}

void multi_gl_view_get_required_version(MultiGLView* self, int *major, int *minor)
{
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	if (major != NULL)
	{
		*major = private->required_gl_version / 10;
	}
	
	if (minor != NULL)
	{
		*minor = private->required_gl_version % 10;
	}
}

void multi_gl_view_set_required_version(MultiGLView* self, int major, int minor)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	g_return_if_fail(!gtk_widget_get_realized(GTK_WIDGET(self)));
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	private->required_gl_version = major * 10 + minor;
}

GdkGLAPI multi_gl_view_get_allowed_apis(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), 0);
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	return private->allowed_apis;
}

void multi_gl_view_set_allowed_apis(MultiGLView* self, GdkGLAPI apis)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	g_return_if_fail(!gtk_widget_get_realized(GTK_WIDGET(self)));
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	if (private->allowed_apis == apis)
	{
		return;
	}
	
	private->allowed_apis = apis;
}

gboolean multi_gl_view_get_auto_render(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), FALSE);
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	return private->auto_render;
}

void multi_gl_view_set_auto_render(MultiGLView* self, gboolean auto_render)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	auto_render = !!auto_render;
	
	if (private->auto_render != auto_render)
	{
		private->auto_render = auto_render;
    }
}

GtkWidget* multi_gl_view_get_view_widget(MultiGLView* self, int index)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), NULL);
	g_return_val_if_fail(index >= 0 && index < 5, NULL);
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
    return index < 4 ? private->small_views[index] : private->maximize_view;
}

void multi_gl_view_add_overlay(MultiGLView* self, int index, GtkWidget* widget)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	g_return_if_fail(index >= 0 && index < 5);
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	if (index < 4)
	{
		gtk_overlay_add_overlay(GTK_OVERLAY(private->small_view_overlays[index]), widget);
	}
	else
	{
		gtk_overlay_add_overlay(GTK_OVERLAY(private->maximize_view_overlay), widget);
	}
	
	gtk_widget_set_halign(widget, GTK_ALIGN_START);
    gtk_widget_set_valign(widget, GTK_ALIGN_START);
    
}


gboolean multi_gl_view_get_maximized(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), FALSE);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	return private->maximized_mode;
}

void multi_gl_view_set_maximized(MultiGLView* self, gboolean maximized)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	private->maximized_mode = maximized;
	
	if (maximized == FALSE)
	{
		gtk_widget_set_visible(private->main_paned, TRUE);
		gtk_widget_set_visible(private->maximize_view_overlay, FALSE);
	}
	else
	{
		gtk_widget_set_visible(private->main_paned, FALSE);
		gtk_widget_set_visible(private->maximize_view_overlay, TRUE);		
	}
	
	gtk_widget_queue_draw(GTK_WIDGET(self));
}

GError* multi_gl_view_get_error(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), NULL);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	return private->error;
}

void multi_gl_view_set_error(MultiGLView* self, const GError* error)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	g_clear_error(&private->error);
	
	if (error)
	{
		private->error = g_error_copy(error);
	}
	else
	{
		private->error = NULL;
	}
}


void multi_gl_view_set_render_callback(MultiGLView* self, RenderCallback render_scene, void* userdata)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	private->render_scene = render_scene;
	private->userdata = userdata;
}

void multi_gl_view_queue_render(MultiGLView* self)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	private->needs_render = TRUE;
	gtk_widget_queue_draw(GTK_WIDGET(self));
}

GdkGLContext* multi_gl_view_get_context(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), NULL);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	return private->context;
}

GdkGLAPI multi_gl_view_get_api(MultiGLView* self)
{
	g_return_val_if_fail(IS_MULTI_GL_VIEW(self), 0);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	if (private->context)
	{
		return gdk_gl_context_get_api(private->context);
	}
	
    return 0;
}

void multi_gl_view_make_current(MultiGLView* self)
{
	g_return_if_fail(IS_MULTI_GL_VIEW(self));
	g_return_if_fail(gtk_widget_get_realized(GTK_WIDGET(self)));
	
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	if (private->context != NULL)
	{
		gdk_gl_context_make_current(GDK_GL_CONTEXT(private->context));
	}
	
}   

GtkWidget* multi_gl_view_new(void) 
{
	MultiGLView* self = g_object_new(MULTI_GL_VIEW_TYPE, NULL);
	MultiGLViewPrivate* private = multi_gl_view_get_instance_private(self);
	
	GtkWidget* Widget = GTK_WIDGET(self);
	gtk_orientable_set_orientation(GTK_ORIENTABLE(self), GTK_ORIENTATION_VERTICAL);
	gtk_widget_set_hexpand(Widget, TRUE);
	gtk_widget_set_vexpand(Widget, TRUE);
	
	private->main_paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
	private->top_paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	private->bottom_paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
	
	gtk_paned_set_wide_handle(GTK_PANED(private->main_paned), TRUE);
	gtk_paned_set_wide_handle(GTK_PANED(private->top_paned), TRUE);
	gtk_paned_set_wide_handle(GTK_PANED(private->bottom_paned), TRUE);
	
	private->small_view_overlays[0] = gtk_overlay_new();
	private->small_view_overlays[1] = gtk_overlay_new();
	private->small_view_overlays[2] = gtk_overlay_new();
	private->small_view_overlays[3] = gtk_overlay_new();
	private->maximize_view_overlay = gtk_overlay_new();	
	
	private->small_views[0] = simple_gl_view_new();
	private->small_views[1] = simple_gl_view_new();
	private->small_views[2] = simple_gl_view_new();
	private->small_views[3] = simple_gl_view_new();
	private->maximize_view = simple_gl_view_new();
	
	gtk_overlay_set_child(GTK_OVERLAY(private->small_view_overlays[0]), private->small_views[0]);
	gtk_overlay_set_child(GTK_OVERLAY(private->small_view_overlays[1]), private->small_views[1]);
	gtk_overlay_set_child(GTK_OVERLAY(private->small_view_overlays[2]), private->small_views[2]);
	gtk_overlay_set_child(GTK_OVERLAY(private->small_view_overlays[3]), private->small_views[3]);
	
	gtk_overlay_set_child(GTK_OVERLAY(private->maximize_view_overlay), private->maximize_view);
	
	gtk_widget_set_hexpand(private->small_views[0], TRUE);
    gtk_widget_set_vexpand(private->small_views[0], TRUE);
    
	gtk_widget_set_hexpand(private->small_views[1], TRUE);
    gtk_widget_set_vexpand(private->small_views[1], TRUE);
	
	gtk_widget_set_hexpand(private->small_views[2], TRUE);
    gtk_widget_set_vexpand(private->small_views[2], TRUE);
    
    gtk_widget_set_hexpand(private->small_views[3], TRUE);
    gtk_widget_set_vexpand(private->small_views[3], TRUE);
    
    gtk_widget_set_hexpand(private->maximize_view, TRUE);
    gtk_widget_set_vexpand(private->maximize_view, TRUE);
    
	gtk_paned_set_start_child(GTK_PANED(private->top_paned), private->small_view_overlays[0]);
	gtk_paned_set_resize_start_child(GTK_PANED(private->top_paned), TRUE);
	gtk_paned_set_shrink_start_child(GTK_PANED(private->top_paned), TRUE);
	
	gtk_paned_set_end_child(GTK_PANED(private->top_paned), private->small_view_overlays[1]);
	gtk_paned_set_resize_end_child(GTK_PANED(private->top_paned), TRUE);
	gtk_paned_set_shrink_end_child(GTK_PANED(private->top_paned), TRUE);
	
	gtk_paned_set_start_child(GTK_PANED(private->bottom_paned), private->small_view_overlays[2]);
	gtk_paned_set_resize_start_child(GTK_PANED(private->bottom_paned), TRUE);
	gtk_paned_set_shrink_start_child(GTK_PANED(private->bottom_paned), TRUE);
	
	gtk_paned_set_end_child(GTK_PANED(private->bottom_paned), private->small_view_overlays[3]);
	gtk_paned_set_resize_end_child(GTK_PANED(private->bottom_paned), TRUE);
	gtk_paned_set_shrink_end_child(GTK_PANED(private->bottom_paned), TRUE);
	
    gtk_paned_set_start_child(GTK_PANED(private->main_paned), private->top_paned);
    gtk_paned_set_resize_start_child(GTK_PANED(private->main_paned), TRUE);
	gtk_paned_set_shrink_start_child(GTK_PANED(private->main_paned), TRUE);
	
    gtk_paned_set_end_child(GTK_PANED(private->main_paned), private->bottom_paned);
	gtk_paned_set_resize_end_child(GTK_PANED(private->main_paned), TRUE);
	gtk_paned_set_shrink_end_child(GTK_PANED(private->main_paned), TRUE);
	
	gtk_box_append(GTK_BOX(self), private->main_paned);
    gtk_box_append(GTK_BOX(self), private->maximize_view_overlay);
    
    gtk_widget_set_visible(private->main_paned, TRUE);
	gtk_widget_set_visible(private->maximize_view_overlay, FALSE);
	
    return Widget;
}

