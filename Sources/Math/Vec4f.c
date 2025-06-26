/*
 * Vec4f.c
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
#include "Vec4f.h"

void Vec4f_Clear(Vec4f* VectorA)
{
	VectorA->X = 0.0f;
	VectorA->Y = 0.0f;
	VectorA->Z = 0.0f;
	VectorA->W = 0.0f;
}

void Vec4f_Add(Vec4f* VectorA, Vec4f* VectorB)
{
	VectorA->X += VectorB->X;
	VectorA->Y += VectorB->Y;
	VectorA->Z += VectorB->Z;
	VectorA->W += VectorB->W;
}

void Vec4f_AddScaled(Vec4f* VectorA, Vec4f* VectorB, float Scale)
{
	VectorA->X += (VectorB->X * Scale);
	VectorA->Y += (VectorB->Y * Scale);
	VectorA->Z += (VectorB->Z * Scale);
	VectorA->W += (VectorB->W * Scale);
}

void Vec4f_Subtract(Vec4f* VectorA, Vec4f* VectorB)
{
	VectorA->X -= VectorB->X;
	VectorA->Y -= VectorB->Y;
	VectorA->Z -= VectorB->Z;
	VectorA->W -= VectorB->W;
}

int Vec4f_Compare(Vec4f* VectorA, Vec4f* VectorB, float Epsilon)
{
	return (fabsf(VectorA->X - VectorB->X) < Epsilon && 
			fabsf(VectorA->Y - VectorB->Y) < Epsilon && 
			fabsf(VectorA->Z - VectorB->Z) < Epsilon &&
			fabsf(VectorA->W - VectorB->W) < Epsilon);
}

int Vec4f_IsFinite(Vec4f* VectorA)
{
	return (isfinite(VectorA->X) && isfinite(VectorA->Y) && isfinite(VectorA->Z) && isfinite(VectorA->W));
}

#if defined(USE_VEC_4F_AS_GENERIC_VECTOR_ITEM) || defined(USE_VEC_4F_AS_GENERIC_ARRAY_ITEM)

	void Vec4f_GenericItem_Init(void* VectorA)
	{
		Vec4f_Clear((Vec4f*) VectorA);
	}

	void Vec4f_GenericItem_Set(void* VectorA, void* VectorB)
	{
		*(Vec4f*)VectorA = *(Vec4f*)VectorB;
	}

	void Vec4f_GenericItem_Swap(void* VectorA, void* VectorB)
	{
		Vec4f temp = *(Vec4f*)VectorA;
		*(Vec4f*)VectorA = *(Vec4f*)VectorB;
		*(Vec4f*)VectorB = temp;
	}

#endif

