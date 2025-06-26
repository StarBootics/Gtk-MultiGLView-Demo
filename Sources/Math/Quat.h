/*
 * Quat.h
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

#ifndef QUAT_H
#define QUAT_H

#include "Vec3f.h"
#include "Mat44f.h"

typedef struct Quat Quat;

struct Quat 
{
	float S;
	float I;
	float J;
	float K;	
};

void Quat_Add(Quat* This, Quat* Other);
void Quat_Subtract(Quat* This, Quat* Other);
void Quat_Product(Quat* This, Quat* Other, Quat* Result);
void Quat_ProductVector(Quat* This, Vec3f* Vector, Vec3f* Result);
void Quat_Multiply(Quat* This, Quat* Other);
void Quat_Inverse(Quat* This);
float Quat_SquareModulus(Quat* This);
float Quat_Modulus(Quat* This);
int Quat_Compare(Quat* This, Quat* Other, float Epsilon);
int Quat_IsFinite(Quat* This);
int Quat_IsUnit(Quat* This);
int Quat_IsValid(Quat* This);
void Quat_Normalize(Quat* This);
void Quat_DirectRotation(Quat* This, Vec3f* NewVector, Vec3f* Vector);
void Quat_Lerp(Quat* This, Quat* Other, float Proportion, Quat* Destination);
void Quat_Slerp(Quat* This, Quat* Other, float Proportion, Quat* Destination);
void Quat_FromAxisAngle(Quat* This, Vec3f* Axis, float Angle);
void Quat_FromVectors(Quat* This, Vec3f* V1, Vec3f* V2);
void Quat_ToRotationMatrix(Quat* This, Mat44f* Rotation);
void Quat_FromRotationMatrix(Quat* This, Mat44f* Rotation);
void Quat_FromEulerAngles(Quat* This, Vec3f* Angles);

#endif


