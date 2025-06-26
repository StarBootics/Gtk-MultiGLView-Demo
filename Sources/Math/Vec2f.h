/*
 * Vec2.h
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

#ifndef VEC_2F_H
#define VEC_2F_H

typedef struct Vec2f
{
	float X,Y;
} Vec2f;

void Vec2f_Clear(Vec2f*);
void Vec2f_Add(Vec2f*, Vec2f*);
void Vec2f_AddScaled(Vec2f*, Vec2f*, float);
void Vec2f_Subtract(Vec2f*, Vec2f*);
void Vec2f_ProductByScalar(Vec2f*, float);
void Vec2f_DivideByScalar(Vec2f*, float);
float Vec2f_Dot(Vec2f*, Vec2f*);
float Vec2f_Modulus(Vec2f*);
void Vec2f_Normalize(Vec2f*);
void Vec2f_Lerp(Vec2f*, Vec2f*, float, Vec2f*);
int Vec2f_Compare(Vec2f*, Vec2f*, float);
int Vec2f_IsFinite(Vec2f*);

#if defined(USE_VEC_2F_AS_GENERIC_VECTOR_ITEM) || defined(USE_VEC_2F_AS_GENERIC_ARRAY_ITEM)
    void Vec2f_GenericItem_Init(void*);
    void Vec2f_GenericItem_Set(void*, void*);
    void Vec2f_GenericItem_Swap(void*, void*);
#endif

#endif

