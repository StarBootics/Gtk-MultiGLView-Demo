/*
 * FramebufferObject.h
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

#ifndef FRAMEBUFFER_OBJECT_H
#define FRAMEBUFFER_OBJECT_H

#include <epoxy/gl.h>

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

typedef enum FramebufferObjectType
{
	FBO_TYPE_SCENE_3D,
	FBO_TYPE_COLOR_OUTPUT
} FramebufferObjectType;

typedef enum FramebufferObjectAttachementType
{
	FBO_ATTACHEMENT_TYPE_COLOR_TEXTURE_RGBA8,
    FBO_ATTACHEMENT_TYPE_COLOR_TEXTURE_RGBA32F,
    FBO_ATTACHEMENT_TYPE_DEPTH_TEXTURE,
    FBO_ATTACHEMENT_TYPE_RENDER_COLOR_BUFFER,
    FBO_ATTACHEMENT_TYPE_RENDER_DEPTH_BUFFER
} FramebufferObjectAttachementType;

typedef enum FramebufferObjectAttachementID
{
	FBO_ATTACHEMENT_ID_COLOR_BUFFER,
	FBO_ATTACHEMENT_ID_BRIGHT_BUFFER,
	FBO_ATTACHEMENT_ID_DEPTH_BUFFER,
	FBO_ATTACHEMENT_ID_COLOR_TEXTURE
}FramebufferObjectAttachementID;

typedef struct FramebufferObjectAttachement
{
	FramebufferObjectAttachementType Type;
	FramebufferObjectAttachementID ID;
	GLuint AttachementID;
} FramebufferObjectAttachement;

typedef struct FramebufferObject FramebufferObject;

struct FramebufferObject
{
	int Width;
	int Height;
	FramebufferObjectType Type;
	int Multisample;
	GLuint Framebuffer;
	FramebufferObjectAttachement Attachements[3];
};

void FramebufferObject_Bind(FramebufferObject*);
void FramebufferObject_Unbind(FramebufferObject*);

void FramebufferObject_ResolveToFbo(FramebufferObject*, FramebufferObject*, GLenum, GLbitfield);
void FramebufferObject_ResolveToExternal(FramebufferObject*, GLuint, int, int, GLenum, GLbitfield);

void FramebufferObject_Rebuilt(FramebufferObject*, int, int);
void FramebufferObject_Wipeout(FramebufferObject*);

void FramebufferObject_Init(FramebufferObject*, int, int, FramebufferObjectType, int);

#endif

