/*
 * FramebufferObject.c
 * 
 * Copyright 2025 Guillaume Saumure <gsaumure@cgocable.ca>
 * 
 * Input program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Input program is distributed in the hope that it will be useful,
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


#include "FramebufferObject.h"

static void FBO_CreateColorBufferAttachment(FramebufferObject* This, FramebufferObjectAttachementID ID, int Attachement)
{
	if (This->Type == FBO_TYPE_SCENE_3D)
	{
		int DestinationIndex = 0;
		
		if (Attachement == GL_COLOR_ATTACHMENT1)
		{
			DestinationIndex = 1;
		}
		
		This->Attachements[DestinationIndex].ID = ID;
		This->Attachements[DestinationIndex].Type = FBO_ATTACHEMENT_TYPE_RENDER_COLOR_BUFFER;
		
		glGenRenderbuffers(1, &This->Attachements[DestinationIndex].AttachementID);
		glBindRenderbuffer(GL_RENDERBUFFER, This->Attachements[DestinationIndex].AttachementID);
		
		if (This->Multisample == TRUE)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, This->Width, This->Height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, This->Width, This->Height);
		}
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, Attachement, GL_RENDERBUFFER, This->Attachements[DestinationIndex].AttachementID);
		
	}
}

static void FBO_CreateDepthBufferAttachment(FramebufferObject* This, FramebufferObjectAttachementID ID)
{
	if (This->Type == FBO_TYPE_SCENE_3D)
	{
		This->Attachements[2].ID = ID;
		This->Attachements[2].Type = FBO_ATTACHEMENT_TYPE_RENDER_DEPTH_BUFFER;
		
		glGenRenderbuffers(1, &This->Attachements[2].AttachementID);
		glBindRenderbuffer(GL_RENDERBUFFER, This->Attachements[2].AttachementID);
		
		if (This->Multisample == TRUE)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, This->Width, This->Height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, This->Width, This->Height);
		}
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,This->Attachements[2].AttachementID);
	}
	
}

static void FBO_CreateTextureRGBA32F(FramebufferObject* This, FramebufferObjectAttachementID ID, int Attachement)
{
	if (This->Type == FBO_TYPE_COLOR_OUTPUT)
	{
		This->Attachements[0].ID = ID;
		glGenTextures(1, &This->Attachements[0].AttachementID);
		glBindTexture(GL_TEXTURE_2D, This->Attachements[0].AttachementID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, This->Width, This->Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, Attachement, GL_TEXTURE_2D, This->Attachements[0].AttachementID, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

static void FBO_Builder(FramebufferObject* This)
{
	const GLenum DrawBuffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	
	glGenFramebuffers(1, &This->Framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, This->Framebuffer);
	
	if (This->Type == FBO_TYPE_SCENE_3D)
	{
		FBO_CreateColorBufferAttachment(This, FBO_ATTACHEMENT_ID_COLOR_BUFFER, GL_COLOR_ATTACHMENT0);
		FBO_CreateColorBufferAttachment(This, FBO_ATTACHEMENT_ID_BRIGHT_BUFFER, GL_COLOR_ATTACHMENT1);
		FBO_CreateDepthBufferAttachment(This, FBO_ATTACHEMENT_ID_DEPTH_BUFFER);
		glDrawBuffers(2, &DrawBuffers[0]);
	}
	else if (This->Type == FBO_TYPE_COLOR_OUTPUT)
	{
		FBO_CreateTextureRGBA32F(This, FBO_ATTACHEMENT_ID_COLOR_TEXTURE, GL_COLOR_ATTACHMENT0);
		glDrawBuffers(1, &DrawBuffers[0]);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void FramebufferObject_Bind(FramebufferObject* Input)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Input->Framebuffer);
    glViewport(0, 0, Input->Width, Input->Height);
}

void FramebufferObject_Unbind(FramebufferObject* Input)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferObject_ResolveToFbo(FramebufferObject* Input, FramebufferObject* Output, GLenum ReadBuffer, GLbitfield Mask)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Output->Framebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Input->Framebuffer);
    glReadBuffer(ReadBuffer);
    glBlitFramebuffer(0, 0, Input->Width, Input->Height, 0, 0, Output->Width, Output->Height, Mask, GL_NEAREST);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferObject_ResolveToExternal(FramebufferObject* Input, GLuint Framebuffer, int Width, int Height, GLenum ReadBuffer, GLbitfield Mask)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Framebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Input->Framebuffer);
    glDrawBuffer(ReadBuffer);
    glBlitFramebuffer(0, 0, Input->Width, Input->Height, 0, 0, Width, Height, Mask, GL_NEAREST);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferObject_Rebuilt(FramebufferObject* This, int Width, int Height)
{
	FramebufferObject_Wipeout(This);
	This->Width = Width;
	This->Height = Height;
	FBO_Builder(This);	
}

void FramebufferObject_Wipeout(FramebufferObject* This)
{
	if (This->Type == FBO_TYPE_SCENE_3D)
	{
		glDeleteRenderbuffers(1, &This->Attachements[0].AttachementID);
		glDeleteRenderbuffers(1, &This->Attachements[1].AttachementID);
		glDeleteRenderbuffers(1, &This->Attachements[2].AttachementID);
	}
	
	if (This->Type == FBO_TYPE_COLOR_OUTPUT)
	{
		glDeleteTextures(1, &This->Attachements[0].AttachementID);
	}
	
	for (int i = 0; i < 3; i++)
	{
		This->Attachements[i].AttachementID = 0;
	}
		
	glDeleteFramebuffers(1, &This->Framebuffer);
	This->Framebuffer = 0;
	
}

void FramebufferObject_Init(FramebufferObject* This, int Width, int Height, FramebufferObjectType Type, int IsMultiSample)
{
	This->Width = Width;
	This->Height = Height;
	This->Type = Type;
	This->Multisample = IsMultiSample;
	FBO_Builder(This);
}
