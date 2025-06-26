/*
 * Vec3f.h
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

#ifndef VEC_3F_H
#define VEC_3F_H

#include "Shaping.h"
#include "Tween.h"

typedef struct Vec3f
{
	float X,Y,Z;
} Vec3f;

void Vec3f_Clear(Vec3f*);
void Vec3f_Add(Vec3f*, Vec3f*);
void Vec3f_AddScaled(Vec3f*, Vec3f*, float);
void Vec3f_Subtract(Vec3f*, Vec3f*);
void Vec3f_ProductByScalar(Vec3f*, float);
void Vec3f_DivideByScalar(Vec3f*, float);
void Vec3f_Cross(Vec3f*, Vec3f*, Vec3f*);
float Vec3f_Dot(Vec3f*, Vec3f*);
float Vec3f_Modulus(Vec3f*);
void Vec3f_Normalize(Vec3f*);
void Vec3f_Lerp(Vec3f*, Vec3f*, Vec3f*, ShapingEffect, float);
void Vec3f_Tween(Vec3f*, Vec3f*, Vec3f*, TweenEffect, float, float);
int Vec3f_Compare(Vec3f*, Vec3f*, float);
int Vec3f_IsFinite(Vec3f*);
void Vec3f_FindPerpendicular(Vec3f*, Vec3f*);
float Vec3f_AngleBetween(Vec3f*, Vec3f*);
float Vec3f_Distance(Vec3f*, Vec3f*);
void Vec3f_Median(Vec3f*, Vec3f*, Vec3f*);
void Vec3f_SetModulus(Vec3f*, float);
void Vec3f_Clamp(Vec3f*, Vec3f*, Vec3f*);

#if defined(USE_VEC_3F_AS_GENERIC_ITEM)
    void Vec3f_GenericItem_Init(void*);
    void Vec3f_GenericItem_Set(void*, void*);
    void Vec3f_GenericItem_Swap(void*, void*);
#endif

#endif

