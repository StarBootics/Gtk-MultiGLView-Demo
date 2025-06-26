/*
 * Vec2f.c
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
#include "Vec2f.h"

void Vec2f_Clear(Vec2f* VectorA)
{
	VectorA->X = 0.0f;
	VectorA->Y = 0.0f;
}

void Vec2f_Add(Vec2f* VectorA, Vec2f* VectorB)
{
	VectorA->X += VectorB->X;
	VectorA->Y += VectorB->Y;
}

void Vec2f_AddScaled(Vec2f* VectorA, Vec2f* VectorB, float Scale)
{
	VectorA->X += (VectorB->X * Scale);
	VectorA->Y += (VectorB->Y * Scale);
}

void Vec2f_Subtract(Vec2f* VectorA, Vec2f* VectorB)
{
	VectorA->X -= VectorB->X;
	VectorA->Y -= VectorB->Y;
}

void Vec2f_ProductByScalar(Vec2f* VectorA, float Scalar)
{
	VectorA->X *= Scalar;
	VectorA->Y *= Scalar;
}

void Vec2f_DivideByScalar(Vec2f* VectorA, float Scalar)
{
	if (Scalar != 0.0f)
	{
		VectorA->X /= Scalar;
		VectorA->Y /= Scalar;
	}
}

float Vec2f_Dot(Vec2f* VectorA, Vec2f* VectorB)
{
	return VectorA->X * VectorB->X + VectorA->Y * VectorB->Y;
}

float Vec2f_Modulus(Vec2f* VectorA)
{
	return sqrt(VectorA->X * VectorA->X + VectorA->Y * VectorA->Y);
}

void Vec2f_Normalize(Vec2f* VectorA)
{
	float Length = Vec2f_Modulus(VectorA);
	
	if (Length > 1e-6f)
	{
		float InvLength = 1.0f / Length;
		VectorA->X *= InvLength;
		VectorA->Y *= InvLength;
	}
}

void Vec2f_Lerp(Vec2f* VectorA, Vec2f* VectorB, float Proportion, Vec2f* InBetween)
{
	InBetween->X = VectorA->X + Proportion * (VectorB->X - VectorA->X);
	InBetween->Y = VectorA->Y + Proportion * (VectorB->Y - VectorA->Y);
}

int Vec2f_Compare(Vec2f* VectorA, Vec2f* VectorB, float Epsilon)
{
	return (fabsf(VectorA->X - VectorB->X) < Epsilon && fabsf(VectorA->Y - VectorB->Y) < Epsilon);
}

int Vec2f_IsFinite(Vec2f* VectorA)
{
	return (isfinite(VectorA->X) && isfinite(VectorA->Y));
}

#if defined(USE_VEC_2F_AS_GENERIC_VECTOR_ITEM) || defined(USE_VEC_2F_AS_GENERIC_ARRAY_ITEM)

	void Vec2f_GenericItem_Init(void* VectorA)
	{
		Vec2f_Clear((Vec2f*) VectorA);
	}

	void Vec2f_GenericItem_Set(void* VectorA, void* VectorB)
	{
		*(Vec2f*)VectorA = *(Vec2f*)VectorB;
	}

	void Vec2f_GenericItem_Swap(void* VectorA, void* VectorB)
	{
		Vec2f temp = *(Vec2f*)VectorA;
		*(Vec2f*)VectorA = *(Vec2f*)VectorB;
		*(Vec2f*)VectorB = temp;
	}

#endif

