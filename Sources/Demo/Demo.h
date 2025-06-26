/*
 * Demo.h
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


#ifndef DEMO_H
#define DEMO_H

#include <gtk/gtk.h>
#include "MultiGLViewGtk.h"
#include "GTimeoutAddFull.h"

#include "RenderingEngine.h"


typedef struct Demo Demo;

struct Demo
{
	GtkApplication *app;
	GtkWidget *window;
	GtkWidget *mainbox;
	GtkWidget *multiglview;
	
	GtkWidget *menubutton[5];
	GtkWidget *popupover[5];
	GtkGesture *gesturedrag[5];
	GtkEventController *event_control_scroll[5];
	
	GTimeoutAddFull TimeoutRefreshRenderer;
	
	int KeepRefreshingRenderer;
	RenderingEngine MasterRenderer;
	
	
};

void Demo_Init(Demo*);
int Demo_Run(Demo*, int, char**);

#endif

