/*
 * SimpleGLViewGtk.c
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

#include "SimpleGLViewGtk.h"

G_DEFINE_TYPE(SimpleGLView, simple_gl_view, GTK_TYPE_WIDGET)

static void simple_gl_view_init(SimpleGLView* self) 
{
    self->gl_texture = NULL;
    self->error = NULL;
}

static void simple_gl_view_dispose(GObject* object) 
{
    SimpleGLView* self = SIMPLE_GL_VIEW(object);
    
    if (self->gl_texture) 
    {
        g_object_unref(self->gl_texture);
        self->gl_texture = NULL;
    }
    
    g_clear_error(&self->error);
    
    G_OBJECT_CLASS(simple_gl_view_parent_class)->dispose(object);
}

static void simple_gl_view_snapshot(GtkWidget* widget, GtkSnapshot* snapshot) 
{
    SimpleGLView* self = SIMPLE_GL_VIEW(widget);
    int w, h, scale;
    
    scale = gtk_widget_get_scale_factor(widget);
    w = gtk_widget_get_width(widget) * scale;
    h = gtk_widget_get_height(widget) * scale;

    if (w == 0 || h == 0)
        return;
    
    if (self->error != NULL)
    {
        PangoLayout *layout;
        int layout_height;
        const GdkRGBA TextColor = {0.0f, 0.0f, 0.0f, 1.0f};
        
        layout = gtk_widget_create_pango_layout(widget, self->error->message);
        pango_layout_set_width(layout, w * PANGO_SCALE);
        pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
        pango_layout_get_pixel_size(layout, NULL, &layout_height);
        
        gtk_snapshot_save(snapshot);
        gtk_snapshot_translate(snapshot, &GRAPHENE_POINT_INIT(0, (h - layout_height) / 2));
        gtk_snapshot_append_layout(snapshot, layout, &TextColor);
        gtk_snapshot_restore(snapshot);
    
        g_object_unref(layout);
        g_clear_error(&self->error); // Clear error
        return;
    }
    
    if (self->gl_texture == NULL)
        return;
    
    graphene_rect_t bounds = GRAPHENE_RECT_INIT(0, 0, w, h);   
    gtk_snapshot_save(snapshot);
    gtk_snapshot_translate(snapshot, &GRAPHENE_POINT_INIT(0, h));
    gtk_snapshot_scale(snapshot, 1, -1);                                                   
    gtk_snapshot_append_texture(snapshot, self->gl_texture, &bounds);
    gtk_snapshot_restore(snapshot);
    
    g_object_unref(self->gl_texture);
    self->gl_texture = NULL; // Clear the pointer
}

static void simple_gl_view_class_init(SimpleGLViewClass* klass) 
{
    GObjectClass* object_class = G_OBJECT_CLASS(klass);
    GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);

    object_class->dispose = simple_gl_view_dispose;
    widget_class->snapshot = simple_gl_view_snapshot;
}

GtkWidget* simple_gl_view_new() 
{
    SimpleGLView* view = g_object_new(SIMPLE_GL_VIEW_TYPE, NULL);
    return GTK_WIDGET(view);
}

void simple_gl_view_set_texture(SimpleGLView* self, GdkTexture* gl_texture)
{
    g_return_if_fail(IS_SIMPLE_GL_VIEW(self));
    
    if (self->gl_texture != gl_texture)
    {
        if (self->gl_texture)
        {
            g_object_unref(self->gl_texture);
		}
		
        self->gl_texture = gl_texture;
        
        if (gl_texture)
        {
            g_object_ref(self->gl_texture);
		}
		
        gtk_widget_queue_draw(GTK_WIDGET(self)); // Trigger redraw
    }
}

void simple_gl_view_set_error(SimpleGLView* self, GError* error)
{
	g_return_if_fail(IS_SIMPLE_GL_VIEW(self));
	self->error = error;
}


