/*
 * Col3f.c
 * 
 * Copyright 2025 Guillaume <guillaume@guillaume-desktop>
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

#include <stdlib.h>
#include <math.h> 
#include <float.h> 

#include "Col3f.h"

void Col3f_Clear(Col3f* This)
{
	This->Red = 0.0f;
	This->Green = 0.0f;
	This->Blue = 0.0f;
}

int Col3f_Compare(Col3f* This, Col3f* Other, float Epsilon)
{
	return (fabsf(This->Red - Other->Red) < Epsilon && 
			fabsf(This->Green - Other->Green) < Epsilon && 
			fabsf(This->Blue - Other->Blue) < Epsilon);
}

int Col3f_IsFinite(Col3f* This)
{
	return (isfinite(This->Red) && isfinite(This->Green) && isfinite(This->Blue));
}

Col3f* Col3f_Copy(Col3f* This)
{
	Col3f* Copy = malloc(sizeof(Col3f));
	
	if (Copy == NULL)
	{
		exit(EXIT_FAILURE);
	}
	
	*(Copy) = *(This);
	
	return Copy;
}

#if defined(USE_COL_3F_AS_GENERIC_VECTOR_ITEM) || defined(USE_COL_3F_AS_GENERIC_ARRAY_ITEM)

	void Col3f_GenericItem_Init(void* ColorA)
	{
		Col3f_Clear((Col3f*) ColorA);
	}

	void Col3f_GenericItem_Set(void* ColorA, void* ColorB)
	{
		*(Col3f*)ColorA = *(Col3f*)ColorB;
	}

	void Col3f_GenericItem_Swap(void* ColorA, void* ColorB)
	{
		Col3f temp = *(Col3f*)ColorA;
		*(Col3f*)ColorA = *(Col3f*)ColorB;
		*(Col3f*)ColorB = temp;
	}

#endif

