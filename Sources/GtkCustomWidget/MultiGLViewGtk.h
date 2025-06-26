/*
 * MultiGLViewGtk.h
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

#ifndef MULTI_GL_VIEW_GTK_H
#define MULTI_GL_VIEW_GTK_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

G_DECLARE_DERIVABLE_TYPE(MultiGLView, multi_gl_view, MULTI, GL_VIEW, GtkBox)

#define MULTI_GL_VIEW_TYPE (multi_gl_view_get_type())
#define MULTI_GL_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MULTI_GL_VIEW_TYPE, MultiGLView))
#define IS_MULTI_GL_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MULTI_GL_VIEW_TYPE))

typedef struct _MultiGLView MultiGLView;
typedef struct _MultiGLViewClass MultiGLViewClass;

struct _MultiGLViewClass
{
	GtkBoxClass parent_class;
	GdkGLContext* (*create_context)(MultiGLView *self);
};

typedef void (*RenderCallback)(MultiGLView*, int, guint, int, int, void*);

GtkWidget* multi_gl_view_new(void);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// In order to create the GdkGLContext, the required version must be set.

void multi_gl_view_get_required_version(MultiGLView* self, int* major, int* minor);
void multi_gl_view_set_required_version(MultiGLView* self, int major, int minor);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// In order to create the GdkGLContext, the allowed api must be set.

GdkGLAPI multi_gl_view_get_allowed_apis(MultiGLView* self);
void multi_gl_view_set_allowed_apis(MultiGLView* self, GdkGLAPI apis);

gboolean multi_gl_view_get_auto_render(MultiGLView* self);
void multi_gl_view_set_auto_render(MultiGLView* self, gboolean auto_render);

GdkGLContext* multi_gl_view_get_context(MultiGLView* self);
GdkGLAPI multi_gl_view_get_api(MultiGLView* self);

GError* multi_gl_view_get_error(MultiGLView* self);
void multi_gl_view_set_error(MultiGLView* self, const GError* error);

GtkWidget* multi_gl_view_get_view_widget(MultiGLView* self, int index);

void multi_gl_view_add_overlay(MultiGLView* self, int index, GtkWidget* Widget);

gboolean multi_gl_view_get_maximized(MultiGLView* self);
void multi_gl_view_set_maximized(MultiGLView* self, gboolean maximized);

void multi_gl_view_set_render_callback(MultiGLView* self, RenderCallback render_scene, void* userdata);

void multi_gl_view_queue_render(MultiGLView* self);
void multi_gl_view_make_current(MultiGLView* self);                                                  

G_END_DECLS

#endif

