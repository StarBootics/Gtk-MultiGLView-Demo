/*
 * ShaderProgram.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ShaderProgram.h"

static char* ShaderProgram_LoadSource(char* PathFileName)
{
	FILE* Stream = fopen(PathFileName, "rb");
	
	if (Stream == NULL)
	{
		fprintf(stderr, "ShaderProgram->LoadSource() : ReadFile failure ! : %s", PathFileName);
		
		return NULL;
	}
	
	fseek(Stream, 0, SEEK_END);
	long fsize = ftell(Stream);
	fseek(Stream, 0, SEEK_SET);

	char* SourceCode = malloc(fsize + 1);
	
	if (SourceCode == NULL)
	{
		fprintf(stderr, "ShaderProgram->LoadSource() : SourceCode allocation failure ! : %s", PathFileName);
		fclose(Stream);
		return NULL;
	}
	
	fread(SourceCode, fsize, 1, Stream);
	SourceCode[fsize] = 0;
	
	fclose(Stream);
	
	return SourceCode;
}

static GLuint ShaderProgram_CompileShader(char* FileName, char* SourceCode, GLint ShaderType)
{
	GLuint ShaderID = glCreateShader(ShaderType);
	const char* Code = SourceCode;
	glShaderSource(ShaderID, 1, &Code, NULL);
	glCompileShader(ShaderID);
	
	GLint IsShaderCompiled;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &IsShaderCompiled);
	
	if (IsShaderCompiled == FALSE)
	{
		GLint MaxLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &MaxLength);
		
		char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
		
		glGetShaderInfoLog(ShaderID, MaxLength, &MaxLength, ErrorMessage);
		fprintf(stderr, "ShaderProgram->CompileShader() : %s : %s", FileName, ErrorMessage);
		free(ErrorMessage);
		glDeleteShader(ShaderID);
		ShaderID = 0;
	}
	
	return ShaderID;
}

char* ShaderProgram_GetProgramName(ShaderProgram* This)
{
	return This->ProgramName; 
}

GLuint ShaderProgram_GetProgramID(ShaderProgram* This)
{
	return This->ProgramID;
}

void ShaderProgram_AddUniform(ShaderProgram* This, char* UniformName)
{
	This->Uniforms.AddBucket(&This->Uniforms, UniformName, -1);
}

void ShaderProgram_CreateComputeShader(ShaderProgram* This, char* SourcePath, char* CSFileName)
{
	char* PathFileName = (char*) malloc(sizeof(char) * (strlen(SourcePath) + strlen(CSFileName) + 1));
	
	if (PathFileName == NULL)
	{
		return;
	}
	
	strcpy(PathFileName, SourcePath);
	strcat(PathFileName, CSFileName);
	
	char* CSSourceCode = ShaderProgram_LoadSource(PathFileName);
	free(PathFileName);
	
	if (CSSourceCode != NULL)
	{
		GLuint ComputeShaderID = ShaderProgram_CompileShader(CSFileName, CSSourceCode, GL_COMPUTE_SHADER);
		
		if (ComputeShaderID >= 1)
		{
			This->ProgramID = glCreateProgram();
			glAttachShader(This->ProgramID, ComputeShaderID);
			glLinkProgram(This->ProgramID);
			
			GLint IsProgramLinked;
			glGetProgramiv(This->ProgramID, GL_LINK_STATUS, &IsProgramLinked);
			
			if (IsProgramLinked == FALSE)
			{				
				GLint MaxLength;
				glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
					
				char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
				
				glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
				
				fprintf(stderr, "ShaderProgram->CreateComputeShader() : %s : %s", This->ProgramName, ErrorMessage);
				free(ErrorMessage);
				
				glDetachShader(This->ProgramID, ComputeShaderID);
				glDeleteShader(ComputeShaderID);
				glDeleteProgram(This->ProgramID);
				
				return;
			}
			
			glValidateProgram(This->ProgramID);
			
			GLint IsProgramValid;
			glGetProgramiv(This->ProgramID, GL_VALIDATE_STATUS, &IsProgramValid);
			
			if (IsProgramValid == FALSE)
			{				
				GLint MaxLength;
				glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
					
				char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
				
				glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
				
				fprintf(stderr, "ShaderProgram->CreateComputeShader() : %s : %s", This->ProgramName, ErrorMessage);
				free(ErrorMessage);
				
				glDetachShader(This->ProgramID, ComputeShaderID);
				glDeleteShader(ComputeShaderID);
				glDeleteProgram(This->ProgramID);
				return;	
			}
			
			if (This->ProgramID >= 1)
			{
				glDeleteShader(ComputeShaderID);
			}
			
		}
		
		free(CSSourceCode);
	}
}

void ShaderProgram_CreateRenderingShader(ShaderProgram* This, char* SourcePath, char* VSFileName, char* GSFileName, char* FSFileName, BindAttribute RemoteBindAttribute)
{
	char* VSPathFileName = (char*) malloc(sizeof(char) * (strlen(SourcePath) + strlen(VSFileName) + 1));
	
	if (VSPathFileName == NULL)
	{
		return;
	}
	
	strcpy(VSPathFileName, SourcePath);
	strcat(VSPathFileName, VSFileName);
	
	GLuint VertexShaderID = 0, GeometryShaderID = 0, FragmentShaderID = 0;
	
	char* VSSourceCode = ShaderProgram_LoadSource(VSPathFileName);
	free(VSPathFileName);
	
	if (VSSourceCode == NULL)
	{
		
		return;
	}
	
	VertexShaderID = ShaderProgram_CompileShader(VSFileName, VSSourceCode, GL_VERTEX_SHADER);
	free(VSSourceCode);
	
	if (GSFileName != NULL)
	{
		char* GSPathFileName = (char*) malloc(sizeof(char) * (strlen(SourcePath) + strlen(GSFileName) + 1));
		
		if (GSPathFileName == NULL)
		{
			glDeleteShader(VertexShaderID);
			return;
		}
		
		strcpy(GSPathFileName, SourcePath);
		strcat(GSPathFileName, GSFileName);
		
		char* GSSourceCode = ShaderProgram_LoadSource(GSPathFileName);
		free(GSPathFileName);
		
		if (GSSourceCode == NULL)
		{
			glDeleteShader(VertexShaderID);
			return;
		}
		
		GeometryShaderID = ShaderProgram_CompileShader(GSFileName, GSSourceCode, GL_GEOMETRY_SHADER);
		free(GSSourceCode);
	}
	
	char* FSPathFileName = (char*) malloc(sizeof(char) * (strlen(SourcePath) + strlen(FSFileName) + 1));
	
	if (FSPathFileName == NULL)
	{
		return;
	}
	
	strcpy(FSPathFileName, SourcePath);
	strcat(FSPathFileName, FSFileName);
	
	char* FSSourceCode = ShaderProgram_LoadSource(FSPathFileName);
	free(FSPathFileName);
	
	if (FSSourceCode == NULL)
	{
		if (GeometryShaderID > 0)
		{
			glDeleteShader(GeometryShaderID);
		}
		
		glDeleteShader(VertexShaderID);
	}
	
	FragmentShaderID = ShaderProgram_CompileShader(FSFileName, FSSourceCode, GL_FRAGMENT_SHADER);
	
	if (VertexShaderID >= 1 && GeometryShaderID == 0 && FragmentShaderID >= 1)
	{
		This->ProgramID = glCreateProgram();
		glAttachShader(This->ProgramID, VertexShaderID);
		glAttachShader(This->ProgramID, FragmentShaderID);
		
		if (RemoteBindAttribute != NULL)
		{
			RemoteBindAttribute(This->ProgramID);
		}
		
		glLinkProgram(This->ProgramID);
		
		GLint IsProgramLinked;
		glGetProgramiv(This->ProgramID, GL_LINK_STATUS, &IsProgramLinked);
		
		if (IsProgramLinked == FALSE)
		{
			GLint MaxLength;
			glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
			
			char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
			
			glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
			
			fprintf(stderr, "ShaderProgram->CreateRenderingShader() : %s : %s", This->ProgramName, ErrorMessage);
			free(ErrorMessage);
				
			glDetachShader(This->ProgramID, VertexShaderID);
			glDetachShader(This->ProgramID, FragmentShaderID);
			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
			
			glDeleteProgram(This->ProgramID);
			
			return;
		}
		
		glValidateProgram(This->ProgramID);
		
		GLint IsProgramValid;
		glGetProgramiv(This->ProgramID, GL_VALIDATE_STATUS, &IsProgramValid);
		
		if (IsProgramValid == FALSE)
		{
			GLint MaxLength;
			glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
					
			char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
			
			glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
			
			fprintf(stderr, "ShaderProgram->CreateRenderingShader() : %s : %s", This->ProgramName, ErrorMessage);
			free(ErrorMessage);
			
			glDetachShader(This->ProgramID, VertexShaderID);
			glDetachShader(This->ProgramID, FragmentShaderID);
			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
			glDeleteProgram(This->ProgramID);
			
			return;
		}
		
		if (This->ProgramID >= 1)
		{
			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);
		}
	}
	else if (VertexShaderID >= 1 && GeometryShaderID >= 1 && FragmentShaderID >= 1)
	{
		This->ProgramID = glCreateProgram();
		glAttachShader(This->ProgramID, VertexShaderID);
		glAttachShader(This->ProgramID, GeometryShaderID);
		glAttachShader(This->ProgramID, FragmentShaderID);
		
		if (RemoteBindAttribute != NULL)
		{
			RemoteBindAttribute(This->ProgramID);
		}
		
		glLinkProgram(This->ProgramID);
		
		GLint IsProgramLinked;
		glGetProgramiv(This->ProgramID, GL_LINK_STATUS, &IsProgramLinked);
		
		if (IsProgramLinked == FALSE)
		{
			GLint MaxLength;
			glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
			
			char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
			
			glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
			
			fprintf(stderr, "ShaderProgram->CreateRenderingShader() : %s : %s", This->ProgramName, ErrorMessage);
			free(ErrorMessage);
			
			glDetachShader(This->ProgramID, VertexShaderID);
			glDetachShader(This->ProgramID, GeometryShaderID);
			glDetachShader(This->ProgramID, FragmentShaderID);
			glDeleteShader(VertexShaderID);
			glDeleteShader(GeometryShaderID);
			glDeleteShader(FragmentShaderID);
			
			glDeleteProgram(This->ProgramID);
			return;	
		}
		
		glValidateProgram(This->ProgramID);
		
		GLint IsProgramValid;
		glGetProgramiv(This->ProgramID, GL_VALIDATE_STATUS, &IsProgramValid);
		
		if (IsProgramValid == FALSE)
		{
			GLint MaxLength;
			glGetProgramiv(This->ProgramID, GL_INFO_LOG_LENGTH, &MaxLength);
					
			char* ErrorMessage = malloc(sizeof(char) * (MaxLength + 1));
			
			glGetProgramInfoLog(This->ProgramID, MaxLength, &MaxLength, ErrorMessage);
			
			fprintf(stderr, "ShaderProgram->CreateRenderingShader() : %s : %s", This->ProgramName, ErrorMessage);
			free(ErrorMessage);
			
			glDetachShader(This->ProgramID, VertexShaderID);
			glDetachShader(This->ProgramID, GeometryShaderID);
			glDetachShader(This->ProgramID, FragmentShaderID);
			glDeleteShader(VertexShaderID);
			glDeleteShader(GeometryShaderID);
			glDeleteShader(FragmentShaderID);
			glDeleteProgram(This->ProgramID);
			return;	
		}
		
		if (This->ProgramID >= 1)
		{
			glDeleteShader(VertexShaderID);
			glDeleteShader(GeometryShaderID);
			glDeleteShader(FragmentShaderID);
		}
	}
}

void ShaderProgram_GetUniformLocations(ShaderProgram* This)
{
	This->Uniforms.ResetBuckets(&This->Uniforms);
	
	while (This->Uniforms.NextBucket(&This->Uniforms))
	{
		char* UniformName = This->Uniforms.GetBucketKey(&This->Uniforms);
		GLint UniformLocation = glGetUniformLocation(This->ProgramID, UniformName);
		
		if (UniformLocation != -1)
		{
			This->Uniforms.UpdateBucket(&This->Uniforms, UniformLocation);
		}
		else
		{
			fprintf(stderr, "ShaderProgram->GetUniformLocations() : Impossible to find the uniform ! : %s : %s", This->ProgramName, UniformName);
		}
		
	}
}

void ShaderProgram_SendUniformMatrix4fv(ShaderProgram* This, char* UniformName, Mat44f* Matrix)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniformMatrix4fv(This->Uniforms.GetBucketValue(&This->Uniforms), 1, GL_FALSE, (float*) Matrix);
	}
	else
	{
		fprintf(stderr, "ShaderProgram->SendUniformMatrix4fv() : Impossible to find the uniform location ! : %s : %s", This->ProgramName, UniformName);
	}
}

void ShaderProgram_SendUniformVec3fv(ShaderProgram* This, char* UniformName, Vec3f* Vector)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform3fv(This->Uniforms.GetBucketValue(&This->Uniforms), 1, (float*) Vector);
	}
}

void ShaderProgram_SendUniformVec2fv(ShaderProgram* This, char* UniformName, Vec2f* Vector)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform2fv(This->Uniforms.GetBucketValue(&This->Uniforms), 1, (float*) Vector);
	}
}

void ShaderProgram_SendUniformCol4fv(ShaderProgram* This, char* UniformName, Col4f* Color)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform4fv(This->Uniforms.GetBucketValue(&This->Uniforms), 1, (float*) Color);
	}
	else
	{
		fprintf(stderr, "ShaderProgram->SendUniformCol4fv() : Impossible to find the uniform location ! : %s : %s", This->ProgramName, UniformName);
	}
}

void ShaderProgram_SendUniformCol3fv(ShaderProgram* This, char* UniformName, Col3f* Color)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform3fv(This->Uniforms.GetBucketValue(&This->Uniforms), 1, (float*) Color);
	}
}

void ShaderProgram_SendUniform1i(ShaderProgram* This, char* UniformName, GLint Value)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform1i(This->Uniforms.GetBucketValue(&This->Uniforms), Value);
	}
}

void ShaderProgram_SendUniform1f(ShaderProgram* This, char* UniformName, GLfloat Value)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform1f(This->Uniforms.GetBucketValue(&This->Uniforms), Value);
	}
}

void ShaderProgram_SendUniform1iv(ShaderProgram* This, char* UniformName, GLint* Values, GLsizei Size)
{
	if (This->Uniforms.LookupBucket(&This->Uniforms, UniformName))
	{
		glUniform1iv(This->Uniforms.GetBucketValue(&This->Uniforms), Size, Values);
	}
}

void ShaderProgram_Wipeout(ShaderProgram* This)
{
	This->Uniforms.Wipeout(&This->Uniforms);
}

void ShaderProgram_Init(ShaderProgram* This, char* ProgramName)
{
	This->GetProgramName = ShaderProgram_GetProgramName;
	This->GetProgramID = ShaderProgram_GetProgramID;
	This->AddUniform = ShaderProgram_AddUniform;
	This->CreateComputeShader = ShaderProgram_CreateComputeShader;
	This->CreateRenderingShader = ShaderProgram_CreateRenderingShader;
	This->GetUniformLocations = ShaderProgram_GetUniformLocations;
	This->SendUniformMatrix4fv = ShaderProgram_SendUniformMatrix4fv;
	This->SendUniformVec3fv = ShaderProgram_SendUniformVec3fv;
	This->SendUniformVec2fv = ShaderProgram_SendUniformVec2fv;
	This->SendUniformCol4fv = ShaderProgram_SendUniformCol4fv;
	This->SendUniformCol3fv = ShaderProgram_SendUniformCol3fv;
	This->SendUniform1i = ShaderProgram_SendUniform1i;
	This->SendUniform1f = ShaderProgram_SendUniform1f;
	This->SendUniform1iv = ShaderProgram_SendUniform1iv;
	This->Wipeout  = ShaderProgram_Wipeout;
	
	This->ProgramName = ProgramName;
	IntegerHashTable_Init(&This->Uniforms, 32);
}





