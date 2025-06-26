/*
 * SimpleGLViewGtk.h
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

#ifndef SIMPLE_GL_VIEW_GTK_H
#define SIMPLE_GL_VIEW_GTK_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SIMPLE_GL_VIEW_TYPE (simple_gl_view_get_type())
#define SIMPLE_GL_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), SIMPLE_GL_VIEW_TYPE, SimpleGLView))
#define IS_SIMPLE_GL_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SIMPLE_GL_VIEW_TYPE))

typedef struct _SimpleGLView SimpleGLView;
typedef struct _SimpleGLViewClass SimpleGLViewClass;

typedef struct _SimpleGLView
{
    GtkWidget parent_instance;
    GdkTexture* gl_texture;
    GError* error;
} SimpleGLView;

typedef struct _SimpleGLViewClass 
{
    GtkWidgetClass parent_class;
} SimpleGLViewClass;

GtkWidget* simple_gl_view_new(void);
void simple_gl_view_set_texture(SimpleGLView* self, GdkTexture* gl_texture);
void simple_gl_view_set_error(SimpleGLView* self, GError* error);

G_END_DECLS

#endif

