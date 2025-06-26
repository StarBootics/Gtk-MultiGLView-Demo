/*
 * FiniteGridShader.h
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


#ifndef FINITE_GRID_SHADER_H
#define FINITE_GRID_SHADER_H

#include "Col4f.h"
#include "Mat44f.h"

#include "ShaderProgram.h"

typedef struct FiniteGridShader FiniteGridShader;

struct FiniteGridShader
{
	ShaderProgram ShaderProg;
	
	void (*Bind)(FiniteGridShader*);
	void (*Unbind)(FiniteGridShader*);
	void (*SendProjectionMatrix)(FiniteGridShader*, Mat44f*);
	
	void (*SendViewMatrix)(FiniteGridShader*, Mat44f*);
	void (*SendGridSize)(FiniteGridShader*, float);
	void (*SendGridCellSize)(FiniteGridShader*, float);
	
	void (*SendGridColorThin)(FiniteGridShader*, Col4f*);
	void (*SendGridColorThick)(FiniteGridShader*, Col4f*);
	void (*SendPlaneID)(FiniteGridShader*, int);
	void (*Initialize)(FiniteGridShader*, char*);
	void (*Wipeout)(FiniteGridShader*);
};

void FiniteGridShader_Init(FiniteGridShader*);

#endif


