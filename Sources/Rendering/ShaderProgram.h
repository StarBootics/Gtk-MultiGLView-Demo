/*
 * ShaderProgram.h
 * 
 * Copyright 2024 Guillaume Saumure <gsaumure@cgocable.ca>
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

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <epoxy/gl.h>

#include "IntegerHashTable.h"
#include "Mat44f.h"
#include "Vec3f.h"
#include "Vec2f.h"
#include "Col3f.h"
#include "Col4f.h"

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

typedef void (*BindAttribute)(GLuint);

typedef struct ShaderProgram ShaderProgram;

struct ShaderProgram
{
	char* ProgramName;
	GLuint ProgramID;
	IntegerHashTable Uniforms;
	
	char* (*GetProgramName)(ShaderProgram*);
	GLuint (*GetProgramID)(ShaderProgram*);
	void (*AddUniform)(ShaderProgram*, char*);
	void (*CreateComputeShader)(ShaderProgram*, char*, char*);
	void (*CreateRenderingShader)(ShaderProgram*, char*, char*, char*, char*, BindAttribute);
	void (*GetUniformLocations)(ShaderProgram*);
	void (*SendUniformMatrix4fv)(ShaderProgram*, char*, Mat44f*);
	void (*SendUniformVec3fv)(ShaderProgram*, char*, Vec3f*);
	void (*SendUniformVec2fv)(ShaderProgram*, char*, Vec2f*);
	void (*SendUniformCol4fv)(ShaderProgram*, char*, Col4f*);
	void (*SendUniformCol3fv)(ShaderProgram*, char*, Col3f*);
	void (*SendUniform1i)(ShaderProgram*, char*, GLint);
	void (*SendUniform1f)(ShaderProgram*, char*, GLfloat);
	void (*SendUniform1iv)(ShaderProgram*, char*, GLint*, GLsizei);
	void (*Wipeout)(ShaderProgram*);
};

void ShaderProgram_Init(ShaderProgram*, char*);


#endif
