/*
 * Vec4f.h
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

#ifndef VEC_4F_H
#define VEC_4F_H

typedef struct Vec4f
{
	float X,Y,Z,W;
} Vec4f;

void Vec4f_Clear(Vec4f*);
void Vec4f_Add(Vec4f*, Vec4f*);
void Vec4f_AddScaled(Vec4f*, Vec4f*, float);
void Vec4f_Subtract(Vec4f*, Vec4f*);
int Vec4f_Compare(Vec4f*, Vec4f*, float);
int Vec4f_IsFinite(Vec4f*);

#if defined(USE_VEC_4F_AS_GENERIC_VECTOR_ITEM) || defined(USE_VEC_4F_AS_GENERIC_ARRAY_ITEM)
    void Vec4f_GenericItem_Init(void*);
    void Vec4f_GenericItem_Set(void*, void*);
    void Vec4f_GenericItem_Swap(void*, void*);
#endif



#endif

