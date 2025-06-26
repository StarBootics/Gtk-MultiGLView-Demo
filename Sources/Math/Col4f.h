/*
 * Col4f.h
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

#ifndef COL_4F_H
#define COL_4F_H

typedef struct Col4f
{
	float Red;
	float Green;
	float Blue;
	float Alpha;
} Col4f;

void Col4f_Clear(Col4f* This);
int Col4f_Compare(Col4f* This, Col4f* Other, float Epsilon);
int Col4f_IsFinite(Col4f* This);

#if defined(USE_COL_4F_AS_GENERIC_VECTOR_ITEM) || defined(USE_COL_4F_AS_GENERIC_ARRAY_ITEM)
    void Col4f_GenericItem_Init(void*);
    void Col4f_GenericItem_Set(void*, void*);
    void Col4f_GenericItem_Swap(void*, void*);
#endif

#endif

