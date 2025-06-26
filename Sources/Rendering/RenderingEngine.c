/*
 * RenderingEngine.c
 * 
 * Copyright 2025 Guillaume Saumure <gsaumure@cgocable.ca>
 * 
 * engine program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * engine program is distributed in the hope that it will be useful,
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
 
#include "Radian.h" 
#include "RenderingEngine.h"

static void openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) 
{
	
	printf("OpenGL Debug Message [%u]: %s\n", id, message);
	
	
    if (severity == GL_DEBUG_SEVERITY_HIGH) 
    {
        printf("Severity: High\n");
    } 
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM) 
    {
        printf("Severity: Medium\n");
    } 
    else if (severity == GL_DEBUG_SEVERITY_LOW) 
    {
        printf("Severity: Low\n");
    } 
    else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) 
    {
        printf("Severity: Notification\n");
    }
}

void RenderingEngine_RefreshGridInfos(RenderingEngine* engine)
{
	engine->ShaderFiniteGrid.Bind(&engine->ShaderFiniteGrid);
	engine->ShaderFiniteGrid.SendGridSize(&engine->ShaderFiniteGrid, engine->GridSize);
	engine->ShaderFiniteGrid.SendGridCellSize(&engine->ShaderFiniteGrid, engine->GridCellSize);
	engine->ShaderFiniteGrid.SendGridColorThin(&engine->ShaderFiniteGrid, &engine->GridColorThin);
	engine->ShaderFiniteGrid.SendGridColorThick(&engine->ShaderFiniteGrid, &engine->GridColorThick);
	engine->ShaderFiniteGrid.Unbind(&engine->ShaderFiniteGrid);
}

void RenderingEngine_RefreshAfterResize(RenderingEngine* engine, ViewViewport ViewID, int Width, int Height)
{
	if (engine->Widths[ViewID] != Width || engine->Heights[ViewID] != Height)
	{
		engine->Widths[ViewID] = Width;
		engine->Heights[ViewID] = Height;
		
		FramebufferObject_Rebuilt(&engine->MultisampleFbo[ViewID], Width, Height);
		FramebufferObject_Rebuilt(&engine->ColorOutputFbo[ViewID], Width, Height);
		FramebufferObject_Rebuilt(&engine->BrightOutputFbo[ViewID], Width, Height);
		
		ViewName Name = engine->ViewportViewNameMapping[ViewID];
		
		engine->Cameras[Name].SetViewWidth(&engine->Cameras[Name], Width);
		engine->Cameras[Name].SetViewHeight(&engine->Cameras[Name], Height);
		
		float AspectRatio = ((float) Width) / ((float) Height);
		
		Mat44f_Perspective(&engine->ProjectionMatrix[Name], Radian(FIELD_OF_VIEW), AspectRatio, NEAR_PLANE, FAR_PLANE);
		Mat44f_Inverse(&engine->ProjectionMatrix[Name], &engine->InvProjectionMatrix[Name]);
	}

}

void RenderingEngine_ViewportViewNameMapping(RenderingEngine* engine, ViewViewport ViewportID, ViewName Name)
{
	engine->ViewportViewNameMapping[ViewportID] = Name;
}

void RenderingEngine_SwitchMode(RenderingEngine* engine, ViewMode Mode, ViewViewport ViewportID)
{
	if (engine->Mode == VIEW_MODE_MULTIPLE_VIEWS && Mode == VIEW_MODE_SINGLE_VIEW && ViewportID < VIEW_VIEWPORT_E)
	{
		engine->Widths[VIEW_VIEWPORT_E] = 0;
		engine->Heights[VIEW_VIEWPORT_E] = 0;
	}
	
	engine->Mode = Mode;
	
}

void RenderingEngine_Render(RenderingEngine* engine, int ViewportID, GLuint FinalFbo, int Width, int Height)
{
	RenderingEngine_RefreshAfterResize(engine, (ViewViewport) ViewportID, Width, Height);
	
	FramebufferObject_Bind(&engine->MultisampleFbo[ViewportID]);
	
	glClearColor(0.30f, 0.30f, 0.30f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	engine->ShaderFiniteGrid.Bind(&engine->ShaderFiniteGrid);

	ViewName ViewID = engine->ViewportViewNameMapping[ViewportID];
	
	if (ViewID == VIEW_PERSPECTIVE || ViewID == VIEW_TOP)
	{
		engine->ShaderFiniteGrid.SendPlaneID(&engine->ShaderFiniteGrid, 0);
	}
	else if (ViewID == VIEW_FRONT || ViewID == VIEW_BACK)
	{
		engine->ShaderFiniteGrid.SendPlaneID(&engine->ShaderFiniteGrid, 1);
	}
	else if (ViewID == VIEW_RIGHT || ViewID == VIEW_LEFT)
	{
		engine->ShaderFiniteGrid.SendPlaneID(&engine->ShaderFiniteGrid, 2);
	}
	
	engine->ShaderFiniteGrid.SendProjectionMatrix(&engine->ShaderFiniteGrid, &engine->ProjectionMatrix[ViewID]);
	engine->Cameras[ViewID].ComputeMatrices(&engine->Cameras[ViewID]);
	
	
	/*if (ViewID == VIEW_PERSPECTIVE)
	{
		Mat44f* ViewMatrix = &engine->ProjectionMatrix[ViewID];
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e11, ViewMatrix->e12, ViewMatrix->e13, ViewMatrix->e14);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e21, ViewMatrix->e22, ViewMatrix->e23, ViewMatrix->e24);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e31, ViewMatrix->e32, ViewMatrix->e33, ViewMatrix->e34);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e41, ViewMatrix->e42, ViewMatrix->e43, ViewMatrix->e44);
		printf(" \n");
	}*/
	
	
	Mat44f* ViewMatrix = engine->Cameras[ViewID].GetViewMatrix(&engine->Cameras[ViewID]);
	
	/*if (ViewID == VIEW_PERSPECTIVE)
	{
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e11, ViewMatrix->e12, ViewMatrix->e13, ViewMatrix->e14);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e21, ViewMatrix->e22, ViewMatrix->e23, ViewMatrix->e24);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e31, ViewMatrix->e32, ViewMatrix->e33, ViewMatrix->e34);
		printf("[%0.3f, %0.3f, %0.3f, %0.3f]\n", ViewMatrix->e41, ViewMatrix->e42, ViewMatrix->e43, ViewMatrix->e44);
	}*/
	
	engine->ShaderFiniteGrid.SendViewMatrix(&engine->ShaderFiniteGrid, ViewMatrix);
	
	// Disable culling for grid
	glDisable(GL_CULL_FACE);

	// Enable depth testing and offset
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(engine->EmptyVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0); 
	glDisable(GL_BLEND);
	
	// Restore state
	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_CULL_FACE); // Re-enable for 3D models
	glCullFace(GL_BACK);
	
	engine->ShaderFiniteGrid.Unbind(&engine->ShaderFiniteGrid);
	
	FramebufferObject_Unbind(&engine->MultisampleFbo[ViewportID]);
	
	FramebufferObject_ResolveToFbo(&engine->MultisampleFbo[ViewportID], &engine->ColorOutputFbo[ViewportID], GL_COLOR_ATTACHMENT0, GL_COLOR_BUFFER_BIT);
	FramebufferObject_ResolveToFbo(&engine->MultisampleFbo[ViewportID], &engine->BrightOutputFbo[ViewportID], GL_COLOR_ATTACHMENT1, GL_COLOR_BUFFER_BIT);
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Do some post-processing effect here
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Finally we blit the framebuffer from our rendering engine to the FinalFbo.
	// Rendering directly to it don't work and I don't understand why.
	
	FramebufferObject_ResolveToExternal(&engine->ColorOutputFbo[ViewportID], FinalFbo, Width, Height, GL_COLOR_ATTACHMENT0, GL_COLOR_BUFFER_BIT);
}

void RenderingEngine_Initialize(RenderingEngine* engine)
{
	if (engine->IsInitialized == FALSE) 
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Makes sure OpenGL calls the callback immediately after errors
		glDebugMessageCallback(openglDebugCallback, NULL);
		
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		glEnable(GL_MULTISAMPLE);
		
		glGenVertexArrays(1, &engine->EmptyVao);
		
		engine->ShaderFiniteGrid.Initialize(&engine->ShaderFiniteGrid, "res/shaders/");
		RenderingEngine_RefreshGridInfos(engine);
		
		for (ViewViewport ViewportID = 0; ViewportID < VIEW_VIEWPORT_MAX; ViewportID++)
		{
			ViewName Name = engine->ViewportViewNameMapping[ViewportID];
			
			if (Name == VIEW_PERSPECTIVE)
			{
				engine->Cameras[Name].RestoreToPerspectiveView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_TOP)
			{
				engine->Cameras[Name].RestoreToTopView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_FRONT)
			{
				engine->Cameras[Name].RestoreToFrontView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_RIGHT)
			{
				engine->Cameras[Name].RestoreToRightView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_BOTTOM)
			{
				engine->Cameras[Name].RestoreToBottomView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_BACK)
			{
				engine->Cameras[Name].RestoreToBackView(&engine->Cameras[Name]);
			}
			else if (Name == VIEW_LEFT)
			{
				engine->Cameras[Name].RestoreToLeftView(&engine->Cameras[Name]);
			}
			
			FramebufferObject_Init(&engine->MultisampleFbo[ViewportID], 400, 300, FBO_TYPE_SCENE_3D, TRUE);
			FramebufferObject_Init(&engine->ColorOutputFbo[ViewportID], 400, 300, FBO_TYPE_COLOR_OUTPUT, FALSE);
			FramebufferObject_Init(&engine->BrightOutputFbo[ViewportID], 400, 300, FBO_TYPE_COLOR_OUTPUT, FALSE);
		}
	
		engine->IsInitialized = TRUE;
	}
}

void RenderingEngine_Wipeout(RenderingEngine* engine)
{
	if (engine->IsInitialized == TRUE)
	{
		glDeleteVertexArrays(1, &engine->EmptyVao);
		
		for (ViewViewport ViewportID = 0; ViewportID < VIEW_VIEWPORT_MAX; ViewportID++)
		{
			FramebufferObject_Wipeout(&engine->MultisampleFbo[ViewportID]);
			FramebufferObject_Wipeout(&engine->ColorOutputFbo[ViewportID]);
			FramebufferObject_Wipeout(&engine->BrightOutputFbo[ViewportID]);
		}
		
		engine->ShaderFiniteGrid.Wipeout(&engine->ShaderFiniteGrid);
		
		engine->IsInitialized = FALSE;
	}
}

void RenderingEngine_Init(RenderingEngine* engine)
{
	engine->IsInitialized = FALSE;
	engine->GridSize = 100.0f;
	engine->GridCellSize = 1.0f;
	
	engine->GridColorThin = (Col4f){0.5f, 0.5f, 0.5f, 1.0f};
	engine->GridColorThick = (Col4f){1.0f, 1.0f, 1.0f, 1.0f};
	engine->Mode = VIEW_MODE_MULTIPLE_VIEWS;
	
	engine->ViewportViewNameMapping[VIEW_VIEWPORT_A] = VIEW_TOP;
	engine->ViewportViewNameMapping[VIEW_VIEWPORT_B] = VIEW_PERSPECTIVE;
	engine->ViewportViewNameMapping[VIEW_VIEWPORT_C] = VIEW_FRONT;
	engine->ViewportViewNameMapping[VIEW_VIEWPORT_D] = VIEW_RIGHT;
	engine->ViewportViewNameMapping[VIEW_VIEWPORT_E] = VIEW_PERSPECTIVE;
	
	CameraControlSettings_Init(&engine->CamCtrlSettings);
	
	FiniteGridShader_Init(&engine->ShaderFiniteGrid);
	
	for (int i = 0; i < VIEW_VIEWPORT_MAX; i++)
	{
		engine->Widths[i] = 0;
		engine->Heights[i] = 0;
	}
	
	for (int i = 0; i < VIEW_MAX; i++)
	{
		Mat44f_Identity(&engine->ProjectionMatrix[i]);
		Mat44f_Identity(&engine->InvProjectionMatrix[i]);
		CameraControl_Init(&engine->Cameras[i], &engine->CamCtrlSettings);
		
	}
	
}

