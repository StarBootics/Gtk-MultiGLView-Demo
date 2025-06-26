/*
 * Vec3.c
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

#include <math.h> 
#include <float.h> 

#include "ClampFloat.h"
#include "Vec3f.h"

void Vec3f_Clear(Vec3f* VectorA)
{
	VectorA->X = 0.0f;
	VectorA->Y = 0.0f;
	VectorA->Z = 0.0f;
}

void Vec3f_Add(Vec3f* VectorA, Vec3f* VectorB)
{
	VectorA->X += VectorB->X;
	VectorA->Y += VectorB->Y;
	VectorA->Z += VectorB->Z;
}

void Vec3f_AddScaled(Vec3f* VectorA, Vec3f* VectorB, float Scale)
{
	VectorA->X += (VectorB->X * Scale);
	VectorA->Y += (VectorB->Y * Scale);
	VectorA->Z += (VectorB->Z * Scale);
}

void Vec3f_Subtract(Vec3f* VectorA, Vec3f* VectorB)
{
	VectorA->X -= VectorB->X;
	VectorA->Y -= VectorB->Y;
	VectorA->Z -= VectorB->Z;
}

void Vec3f_ProductByScalar(Vec3f* VectorA, float Scalar)
{
	VectorA->X *= Scalar;
	VectorA->Y *= Scalar;
	VectorA->Z *= Scalar;
}

void Vec3f_DivideByScalar(Vec3f* VectorA, float Scalar)
{
	if (Scalar != 0.0f)
	{
		VectorA->X /= Scalar;
		VectorA->Y /= Scalar;
		VectorA->Z /= Scalar;
	}
}

void Vec3f_Cross(Vec3f* VectorA, Vec3f* VectorB, Vec3f* Result)
{
	Result->X = VectorA->Y * VectorB->Z - VectorA->Z * VectorB->Y;
	Result->Y = VectorA->Z * VectorB->X - VectorA->X * VectorB->Z;
	Result->Z = VectorA->X * VectorB->Y - VectorA->Y * VectorB->X;
}

float Vec3f_Dot(Vec3f* VectorA, Vec3f* VectorB)
{
	return VectorA->X * VectorB->X + VectorA->Y * VectorB->Y + VectorA->Z * VectorB->Z;
}

float Vec3f_Modulus(Vec3f* VectorA)
{
	return sqrt(VectorA->X * VectorA->X + VectorA->Y * VectorA->Y + VectorA->Z * VectorA->Z);
}

void Vec3f_Normalize(Vec3f* VectorA)
{
	float Length = Vec3f_Modulus(VectorA);
	
	if (Length > 1e-6f)
	{
		float InvLength = 1.0f / Length;
		VectorA->X *= InvLength;
		VectorA->Y *= InvLength;
		VectorA->Z *= InvLength;
	}
}

void Vec3f_Lerp(Vec3f* VectorA, Vec3f* VectorB, Vec3f* InBetween, ShapingEffect Effect, float Proportion)
{
	Proportion = Shaping(Effect, Proportion);
	InBetween->X = VectorA->X + Proportion * (VectorB->X - VectorA->X);
	InBetween->Y = VectorA->Y + Proportion * (VectorB->Y - VectorA->Y);
	InBetween->Z = VectorA->Z + Proportion * (VectorB->Z - VectorA->Z);
}

void Vec3f_Tween(Vec3f* VectorA, Vec3f* VectorB, Vec3f* InBetween, TweenEffect Effect, float CurrentTime, float Duration)
{
	InBetween->X = Tween(Effect, CurrentTime, VectorA->X, VectorB->X - VectorA->X, Duration);
	InBetween->Y = Tween(Effect, CurrentTime, VectorA->Y, VectorB->Y - VectorA->Y, Duration);
	InBetween->Z = Tween(Effect, CurrentTime, VectorA->Z, VectorB->Z - VectorA->Z, Duration);
}

int Vec3f_Compare(Vec3f* VectorA, Vec3f* VectorB, float Epsilon)
{
	return (fabsf(VectorA->X - VectorB->X) < Epsilon && fabsf(VectorA->Y - VectorB->Y) < Epsilon && fabsf(VectorA->Z - VectorB->Z) < Epsilon);
}

int Vec3f_IsFinite(Vec3f* VectorA)
{
	return (isfinite(VectorA->X) && isfinite(VectorA->Y) && isfinite(VectorA->Z));
}

void Vec3f_FindPerpendicular(Vec3f* VectorA, Vec3f* Perpendicular)
{
	Vec3f i = (Vec3f){1.0f, 0.0f, 0.0f};
	Vec3f j = (Vec3f){0.0f, 1.0f, 0.0f};
	Vec3f k = (Vec3f){0.0f, 0.0f, 1.0f};
	
	float id = Vec3f_Dot(VectorA, &i);
	float jd = Vec3f_Dot(VectorA, &j);
	float kd = Vec3f_Dot(VectorA, &k);
	
	if (fabsf(id) <= fabsf(jd) && fabsf(id) <= fabsf(kd))
	{
		if (id < 0.0f)
		{
			i.X *= -1.0f;
		}
		
		Vec3f_Cross(VectorA, &i, Perpendicular);
	}		
	else
	{	
		if (fabsf(jd) <= fabsf(id) && fabsf(jd) <= fabsf(kd))
		{		
			if (jd < 0.0f)
			{
				j.Y *= -1.0f;
			}
			
			Vec3f_Cross(VectorA, &j, Perpendicular);
		}
		else
		{	
			if (kd < 0.0f)
			{
				k.Z *= -1.0f;
			}
				
			Vec3f_Cross(VectorA, &k, Perpendicular);	
		}
			
	}
	
	Vec3f_Normalize(Perpendicular);
	
}

float Vec3f_AngleBetween(Vec3f* VectorA, Vec3f* VectorB)
{
	float VectorAModulus = Vec3f_Modulus(VectorA);
	float VectorBModulus = Vec3f_Modulus(VectorB);
	float DotP = Vec3f_Dot(VectorA, VectorB);
	float AngleBetween = 0.0f;
	const float Epsilon = 1e-6f;
	 
	if (VectorAModulus > Epsilon && VectorBModulus > Epsilon)
	{
		float cosTheta = fmaxf(-1.0f, fminf(1.0f, DotP / (VectorAModulus * VectorBModulus)));
		AngleBetween = acosf(cosTheta);
	}
	
	return AngleBetween;
}

float Vec3f_Distance(Vec3f* VectorA, Vec3f* VectorB)
{
	float x = VectorA->X - VectorB->X;
	float y = VectorA->Y - VectorB->Y;
	float z = VectorA->Z - VectorB->Z;
	
	return sqrtf(x * x + y * y + z * z);
}

void Vec3f_Median(Vec3f* VectorA, Vec3f* VectorB, Vec3f* Median)
{
    Median->X = (VectorA->X + VectorB->X) / 2.0f;
    Median->Y = (VectorA->Y + VectorB->Y) / 2.0f;
    Median->Z = (VectorA->Z + VectorB->Z) / 2.0f;
}

void Vec3f_SetModulus(Vec3f* VectorA, float Modulus)
{
	if (Modulus > 0.0f)
	{
		Vec3f_Normalize(VectorA);
		VectorA->X *= Modulus;
		VectorA->Y *= Modulus;
		VectorA->Z *= Modulus;
	}
	else
	{
		VectorA->X = 0.0f;
		VectorA->Y = 0.0f;
		VectorA->Z = 0.0f;
	}
}

void Vec3f_Min(Vec3f* VectorA, Vec3f* VectorB)
{
	float MinX = VectorA->X;
	
	if (VectorB->X < MinX)
	{
		MinX = VectorB->X;
	}
	
	float MinY = VectorA->Y;
	
	if (VectorB->Y < MinY)
	{
		MinY = VectorB->Y;
	}
	
	float MinZ = VectorA->Z;
	
	if (VectorB->Z < MinZ)
	{
		MinZ = VectorB->Z;
	}
	
	VectorA->X = MinX;
	VectorA->Y = MinY;
	VectorA->Z = MinZ;
	
}

void Vec3f_Max(Vec3f* VectorA, Vec3f* VectorB)
{
	float MaxX = VectorA->X;
	
	if (VectorB->X > MaxX)
	{
		MaxX = VectorB->X;
	}
	
	float MaxY = VectorA->Y;
	
	if (VectorB->Y > MaxY)
	{
		MaxY = VectorB->Y;
	}
	
	float MaxZ = VectorA->Z;
	
	if (VectorB->Z > MaxZ)
	{
		MaxZ = VectorB->Z;
	}
	
	VectorA->X = MaxX;
	VectorA->Y = MaxY;
	VectorA->Z = MaxZ;
	
}

void Vec3f_Clamp(Vec3f* This, Vec3f* Min, Vec3f* Max)
{
	This->X = ClampFloat(This->X, Min->X, Max->X);
	This->Y = ClampFloat(This->Y, Min->Y, Max->Y);
	This->Z = ClampFloat(This->Z, Min->Z, Max->Z);
}

#if defined(USE_VEC_3F_AS_GENERIC_ITEM) 

	void Vec3f_GenericItem_Init(void* VectorA)
	{
		Vec3f_Clear((Vec3f*) VectorA);
	}

	void Vec3f_GenericItem_Set(void* VectorA, void* VectorB)
	{
		*(Vec3f*)VectorA = *(Vec3f*)VectorB;
	}

	void Vec3f_GenericItem_Swap(void* VectorA, void* VectorB)
	{
		Vec3f temp = *(Vec3f*)VectorA;
		*(Vec3f*)VectorA = *(Vec3f*)VectorB;
		*(Vec3f*)VectorB = temp;
	}

#endif
