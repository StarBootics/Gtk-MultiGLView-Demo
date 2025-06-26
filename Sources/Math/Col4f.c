/*
 * Col4f.c
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

#include <math.h> 
#include <float.h> 
#include "Col4f.h"

void Col4f_Clear(Col4f* This)
{
	This->Red = 0.0f;
	This->Green = 0.0f;
	This->Blue = 0.0f;
	This->Alpha = 0.0f;
}

int Col4f_Compare(Col4f* This, Col4f* Other, float Epsilon)
{
	return (fabsf(This->Red - Other->Red) < Epsilon && 
			fabsf(This->Green - Other->Green) < Epsilon && 
			fabsf(This->Blue - Other->Blue) < Epsilon &&
			fabsf(This->Alpha - Other->Alpha) < Epsilon);
}

int Col4f_IsFinite(Col4f* This)
{
	return (isfinite(This->Red) && isfinite(This->Green) && isfinite(This->Blue) && isfinite(This->Alpha));
}

#if defined(USE_COL_4F_AS_GENERIC_VECTOR_ITEM) || defined(USE_COL_4F_AS_GENERIC_ARRAY_ITEM)

	void Col4f_GenericItem_Init(void* ColorA)
	{
		Col4f_Clear((Col4f*) ColorA);
	}

	void Col4f_GenericItem_Set(void* ColorA, void* ColorB)
	{
		*(Col4f*)ColorA = *(Col4f*)ColorB;
	}

	void Col4f_GenericItem_Swap(void* ColorA, void* ColorB)
	{
		Col4f temp = *(Col4f*)ColorA;
		*(Col4f*)ColorA = *(Col4f*)ColorB;
		*(Col4f*)ColorB = temp;
	}

#endif

