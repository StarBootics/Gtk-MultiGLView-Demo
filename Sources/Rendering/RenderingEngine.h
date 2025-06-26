/*
 * RenderingEngine.h
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

#ifndef RENDERING_ENGINE_H
#define RENDERING_ENGINE_H

#include <epoxy/gl.h>

#include "Col4f.h"
#include "Mat44f.h"

#include "CameraControlSettings.h"
#include "CameraControl.h"

#include "FramebufferObject.h"
#include "FiniteGridShader.h"

#define FIELD_OF_VIEW 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

typedef enum
{
	VIEW_MODE_MULTIPLE_VIEWS,
	VIEW_MODE_SINGLE_VIEW
} ViewMode;

typedef enum
{
	VIEW_VIEWPORT_A,
	VIEW_VIEWPORT_B,
	VIEW_VIEWPORT_C,
	VIEW_VIEWPORT_D,
	VIEW_VIEWPORT_E,
	VIEW_VIEWPORT_MAX
} ViewViewport;

typedef enum
{
	VIEW_PERSPECTIVE,
	VIEW_FRONT,
	VIEW_BACK,
	VIEW_TOP,
	VIEW_BOTTOM,
	VIEW_LEFT,
	VIEW_RIGHT,
	VIEW_MAX
} ViewName;

typedef struct RenderingEngine RenderingEngine;

struct RenderingEngine
{
	int IsInitialized;
	float GridSize;
	float GridCellSize;
	Col4f GridColorThin;
	Col4f GridColorThick;
	CameraControlSettings CamCtrlSettings;
	
	ViewMode Mode;
	ViewName ViewportViewNameMapping[VIEW_VIEWPORT_MAX];
	FramebufferObject MultisampleFbo[VIEW_VIEWPORT_MAX];
	FramebufferObject ColorOutputFbo[VIEW_VIEWPORT_MAX];
	FramebufferObject BrightOutputFbo[VIEW_VIEWPORT_MAX];
	int Widths[VIEW_VIEWPORT_MAX];
	int Heights[VIEW_VIEWPORT_MAX];
	
	Mat44f ProjectionMatrix[VIEW_MAX];
	Mat44f InvProjectionMatrix[VIEW_MAX];
	CameraControl Cameras[VIEW_MAX];
	
	FiniteGridShader ShaderFiniteGrid;
	
	GLuint EmptyVao;
	
};

void RenderingEngine_ViewportViewNameMapping(RenderingEngine*, ViewViewport, ViewName);
void RenderingEngine_SwitchMode(RenderingEngine*, ViewMode, ViewViewport);
void RenderingEngine_Render(RenderingEngine*, int, GLuint, int, int);
void RenderingEngine_Initialize(RenderingEngine*);
void RenderingEngine_Wipeout(RenderingEngine*);
void RenderingEngine_Init(RenderingEngine*);

#endif

