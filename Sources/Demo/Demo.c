/*
 * Demo.c
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


#include "Demo.h"

static int KeepRefreshingRenderer(void* user_data)
{
	Demo* demo = (Demo*) user_data;
	
	if (demo->KeepRefreshingRenderer == TRUE)
	{
		for (ViewName Index = VIEW_PERSPECTIVE; Index < VIEW_MAX; Index++)
		{
			demo->MasterRenderer.Cameras[Index].Update(&demo->MasterRenderer.Cameras[Index], 0.016);
		}
		
		multi_gl_view_queue_render(MULTI_GL_VIEW(demo->multiglview));
	}
	
	return demo->KeepRefreshingRenderer;
}

static GMenuModel* Demo_CreateMenu(Demo* This, ViewViewport Index);

static void on_menu_item_activate(GSimpleAction *action, GVariant *parameter, gpointer user_data) 
{
	Demo* demo = (Demo*) user_data;
	
    const char* Name = g_action_get_name(G_ACTION(action));
  
    if (strcmp(Name, "MaximizeTop") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_TOP);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_A);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Top");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizePerspective") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_PERSPECTIVE);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_B);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Perspective");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizeBottom") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_BOTTOM);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_A);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Bottom");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizeFront") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_FRONT);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_C);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Front");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizeBack") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_BACK);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_C);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Back");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizeRight") == 0)
    {		
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_RIGHT);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_D);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Right");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "MaximizeLeft") == 0)
    {
		RenderingEngine_ViewportViewNameMapping(&demo->MasterRenderer, VIEW_VIEWPORT_E, VIEW_LEFT);
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_SINGLE_VIEW, VIEW_VIEWPORT_D);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), TRUE);
		gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[4]), "Left");
		gtk_popover_menu_set_menu_model(GTK_POPOVER_MENU(demo->popupover[4]), Demo_CreateMenu(demo, VIEW_VIEWPORT_E));
	}
	
    if (strcmp(Name, "UnMaximizeView") == 0)
    {
		RenderingEngine_SwitchMode(&demo->MasterRenderer, VIEW_MODE_MULTIPLE_VIEWS, VIEW_VIEWPORT_E);
		multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), FALSE);
	}
	
    g_print("Menu item %s activated\n", Name);
}


static GMenuModel* Demo_CreateMenu(Demo* This, ViewViewport Index)
{
    GMenu *menu = g_menu_new();
    const char* AppActionNames[5];
    const char* ActionNames[5];
    const char* MenuLabels[5];
    
	switch (This->MasterRenderer.ViewportViewNameMapping[Index])
	{
		case VIEW_PERSPECTIVE:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizePerspective";
				ActionNames[0] = "MaximizePerspective";
				MenuLabels[0] = "Maximize Perspective";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}
			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			
			break;
			
		case VIEW_TOP:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeTop";
				ActionNames[0] = "MaximizeTop";
				MenuLabels[0] = "Maximize Top";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}
			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_FRONT:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeFront";
				ActionNames[0] = "MaximizeFront";
				MenuLabels[0] = "Maximize Front";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}
			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_RIGHT:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeRight";
				ActionNames[0] = "MaximizeRight";
				MenuLabels[0] = "Maximize Right";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}
			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_BOTTOM:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeBottom";
				ActionNames[0] = "MaximizeBottom";
				MenuLabels[0] = "Maximize Bottom";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}

			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_BACK:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeBack";
				ActionNames[0] = "MaximizeBack";
				MenuLabels[0] = "Maximize Back";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}

			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_LEFT:
			if (This->MasterRenderer.Mode == VIEW_MODE_MULTIPLE_VIEWS)
			{
				AppActionNames[0] = "app.MaximizeLeft";
				ActionNames[0] = "MaximizeLeft";
				MenuLabels[0] = "Maximize Left";
			}
			else if (This->MasterRenderer.Mode == VIEW_MODE_SINGLE_VIEW)
			{
				AppActionNames[0] = "app.UnMaximizeView";
				ActionNames[0] = "UnMaximizeView";
				MenuLabels[0] = "Un-Maximize View";
			}

			
			AppActionNames[1] = "app.item1";
			ActionNames[1] = "item1";
			MenuLabels[1] = "Item 1";
			
			AppActionNames[2] = "app.item2";
			ActionNames[2] = "item2";
			MenuLabels[2] = "Item 2";
			break;
			
		case VIEW_MAX: break;
	}
	
	for (int i = 0; i < 3; i++) {
		g_menu_append(menu, MenuLabels[i], AppActionNames[i]);
		GSimpleAction *action = g_simple_action_new(ActionNames[i], NULL);
		g_signal_connect(action, "activate", G_CALLBACK(on_menu_item_activate), This);
		g_action_map_add_action(G_ACTION_MAP(This->app), G_ACTION(action));
		g_object_unref(action);
	}
			
    return G_MENU_MODEL(menu);
}

static void Demo_OnRender(MultiGLView *area, int Index, guint Fbo, int Width, int Height, gpointer user_data) 
{
	Demo* demo = (Demo*) user_data;
	RenderingEngine_Render(&demo->MasterRenderer, Index, Fbo, Width, Height);
}

static void Demo_OnRealize(GtkWidget* Widget, void* user_data)
{
	Demo* demo = (Demo*) user_data;
	
	GError* error = multi_gl_view_get_error(MULTI_GL_VIEW(demo->multiglview));
	
	if (error != NULL)
	{
		printf("%s\n", error->message);
		return;
	}
	
	multi_gl_view_make_current(MULTI_GL_VIEW(demo->multiglview));
	
	RenderingEngine_Initialize(&demo->MasterRenderer);
	
	demo->TimeoutRefreshRenderer.Launch(&demo->TimeoutRefreshRenderer);
}

static void Demo_OnUnrealize(GtkWidget* Widget, void* user_data)
{
	Demo* demo = (Demo*) user_data;
	
	multi_gl_view_make_current(MULTI_GL_VIEW(demo->multiglview));
	demo->KeepRefreshingRenderer = FALSE;
	RenderingEngine_Wipeout(&demo->MasterRenderer);
}

static void Demo_Activate(GtkApplication *app, gpointer user_data)
{
	Demo* demo = (Demo*) user_data;
	demo->window = gtk_window_new();
	gtk_application_add_window(demo->app, GTK_WINDOW(demo->window));
	//gtk_application_window_new (demo->app);
	gtk_window_set_title (GTK_WINDOW (demo->window), "MultiGLView Demo");
	gtk_window_set_default_size (GTK_WINDOW (demo->window), 800, 800);
	gtk_widget_set_size_request(demo->window, 800, 800);
	gtk_window_set_resizable(GTK_WINDOW(demo->window), TRUE);
	
	demo->mainbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	
	gtk_widget_set_hexpand(demo->mainbox, TRUE);
	gtk_widget_set_vexpand(demo->mainbox, TRUE);
	
	gtk_widget_set_halign(demo->mainbox, GTK_ALIGN_FILL);
	gtk_widget_set_valign(demo->mainbox, GTK_ALIGN_FILL);

	
	gtk_widget_set_margin_start(demo->mainbox, 5);
	gtk_widget_set_margin_end(demo->mainbox, 5);
	gtk_widget_set_margin_top(demo->mainbox, 5);
	gtk_widget_set_margin_bottom(demo->mainbox, 5);
	
	demo->multiglview = multi_gl_view_new();
	multi_gl_view_set_required_version(MULTI_GL_VIEW(demo->multiglview), 4, 3);
	multi_gl_view_set_allowed_apis(MULTI_GL_VIEW(demo->multiglview), GDK_GL_API_GL);
	multi_gl_view_set_render_callback(MULTI_GL_VIEW(demo->multiglview), Demo_OnRender, demo);
	multi_gl_view_set_maximized(MULTI_GL_VIEW(demo->multiglview), FALSE);
	g_signal_connect(G_OBJECT(demo->multiglview), "realize", G_CALLBACK(Demo_OnRealize), demo);
	g_signal_connect(G_OBJECT(demo->multiglview), "unrealize", G_CALLBACK(Demo_OnUnrealize), demo);
	
	for (int i = 0; i < 5; i++)
	{
		demo->menubutton[i] = gtk_menu_button_new();
		demo->popupover[i] = gtk_popover_menu_new_from_model(Demo_CreateMenu(demo, i));
		gtk_menu_button_set_popover(GTK_MENU_BUTTON(demo->menubutton[i]), demo->popupover[i]); 
		multi_gl_view_add_overlay(MULTI_GL_VIEW(demo->multiglview), i, demo->menubutton[i]);
	}
	
	gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[0]), "Top");
	gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[1]), "Perspective");
	gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[2]), "Front");
	gtk_menu_button_set_label(GTK_MENU_BUTTON(demo->menubutton[3]), "Right");
	
	gtk_box_append(GTK_BOX(demo->mainbox), demo->multiglview);
	gtk_window_set_child(GTK_WINDOW (demo->window), demo->mainbox);
	gtk_window_present (GTK_WINDOW (demo->window));
}

/*static void Demo_Shutdown(GtkApplication* App, void* user_data)
{
	Demo* demo = (Demo*) user_data;
	
}*/

void Demo_Init(Demo* demo)
{
	demo->app = NULL;
	demo->window = NULL;
	demo->multiglview = NULL;
	demo->KeepRefreshingRenderer = TRUE;
	
	RenderingEngine_Init(&demo->MasterRenderer);
	
	GTimeoutAddFull_Init(&demo->TimeoutRefreshRenderer, 0, 16, KeepRefreshingRenderer, demo);
	
}


int Demo_Run(Demo* demo, int argc, char **argv)
{
	int status;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// This as a workaround to force the GL Context creation 
	// and realization for the NVIDIA driver.
	
	g_setenv("GDK_DEBUG", "gl-prefer-gl", TRUE);
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	demo->app = gtk_application_new ("multi.gl.view.example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(demo->app, "activate", G_CALLBACK (Demo_Activate), demo);
	//g_signal_connect(demo->app, "shutdown", G_CALLBACK (Demo_Shutdown), demo);
	status = g_application_run (G_APPLICATION (demo->app), argc, argv);
	g_object_unref (demo->app);
	
	return status;
}

