/*
 * FiniteGridShader.c
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


#include "FiniteGridShader.h"

static void FiniteGridShader_BindAttribute(GLuint ProgramID)
{
    glBindFragDataLocation(ProgramID, 0, "FragColor");
    glBindFragDataLocation(ProgramID, 1, "BrightColor");
}

void FiniteGridShader_Bind(FiniteGridShader* This)
{
	glUseProgram(This->ShaderProg.GetProgramID(&This->ShaderProg));
}

void FiniteGridShader_Unbind(FiniteGridShader* This)
{
	glUseProgram(0);
}

void FiniteGridShader_SendProjectionMatrix(FiniteGridShader* This, Mat44f* Matrix)
{
	This->ShaderProg.SendUniformMatrix4fv(&This->ShaderProg, "ProjectionMatrix", Matrix);
}

void FiniteGridShader_SendViewMatrix(FiniteGridShader* This, Mat44f* Matrix)
{
	This->ShaderProg.SendUniformMatrix4fv(&This->ShaderProg, "ViewMatrix", Matrix);
}

void FiniteGridShader_SendGridSize(FiniteGridShader* This, float Value)
{
	This->ShaderProg.SendUniform1f(&This->ShaderProg, "GridSize", Value);
}

void FiniteGridShader_SendGridCellSize(FiniteGridShader* This, float Value)
{
	This->ShaderProg.SendUniform1f(&This->ShaderProg, "GridCellSize", Value);
}

void FiniteGridShader_SendGridColorThin(FiniteGridShader* This, Col4f* Color)
{
	This->ShaderProg.SendUniformCol4fv(&This->ShaderProg, "GridColorThin", Color);
}

void FiniteGridShader_SendGridColorThick(FiniteGridShader* This, Col4f* Color)
{
	This->ShaderProg.SendUniformCol4fv(&This->ShaderProg, "GridColorThick", Color);
}

void FiniteGridShader_SendPlaneID(FiniteGridShader* This, int Value)
{
	This->ShaderProg.SendUniform1i(&This->ShaderProg, "PlaneID", Value);
}

void FiniteGridShader_Initialize(FiniteGridShader* This, char* Path)
{
	This->ShaderProg.CreateRenderingShader(&This->ShaderProg, Path, "FiniteGrid-vs.glsl", NULL, "FiniteGrid-fs.glsl", FiniteGridShader_BindAttribute);
	
	This->Bind(This);
	This->ShaderProg.GetUniformLocations(&This->ShaderProg);
	This->Unbind(This);
}

void FiniteGridShader_Wipeout(FiniteGridShader* This)
{
	This->ShaderProg.Wipeout(&This->ShaderProg);
}

void FiniteGridShader_Init(FiniteGridShader* This)
{
	This->Bind = FiniteGridShader_Bind;
	This->Unbind = FiniteGridShader_Unbind;
	This->SendProjectionMatrix = FiniteGridShader_SendProjectionMatrix;
	
	This->SendViewMatrix = FiniteGridShader_SendViewMatrix;
	This->SendGridSize = FiniteGridShader_SendGridSize;
	This->SendGridCellSize = FiniteGridShader_SendGridCellSize;
	
	This->SendGridColorThin = FiniteGridShader_SendGridColorThin;
	This->SendGridColorThick = FiniteGridShader_SendGridColorThick;
	This->SendPlaneID = FiniteGridShader_SendPlaneID;
	
	This->Initialize = FiniteGridShader_Initialize;
	This->Wipeout = FiniteGridShader_Wipeout;
	
	ShaderProgram_Init(&This->ShaderProg, "FiniteGrid");
	
	This->ShaderProg.AddUniform(&This->ShaderProg, "ProjectionMatrix");
	This->ShaderProg.AddUniform(&This->ShaderProg, "ViewMatrix");
	This->ShaderProg.AddUniform(&This->ShaderProg, "GridSize");
	This->ShaderProg.AddUniform(&This->ShaderProg, "GridCellSize");
	This->ShaderProg.AddUniform(&This->ShaderProg, "GridColorThin");
	This->ShaderProg.AddUniform(&This->ShaderProg, "GridColorThick");
	This->ShaderProg.AddUniform(&This->ShaderProg, "PlaneID");
}



