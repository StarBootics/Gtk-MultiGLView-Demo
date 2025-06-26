/*
 * Mat44f.h
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

#ifndef MAT_44F_H
#define MAT_44F_H

#include "Vec3f.h" 
#include "Vec4f.h" 

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * Notes : Mat44f
 *
 * La matrice se présente sous la forme suivante :
 *
 * Éléments Matrice     -->  Index du tableau 1D
 * 4x4 = 16 éléments    -->  16 case de 0 à 15
 *  
 * [e11, e12, e13, e14] -->  [00, 04, 08, 12] 
 * [e21, e22, e23, e24] -->  [01, 05, 09, 13]
 * [e31, e32, e33, e34] -->  [02, 06, 10, 14]
 * [e41, e42, e43, e44] -->  [03, 07, 11, 15]
 *
 * Cette disposition des valeurs assure la 
 * compatibilité avec les calculs matriciel 
 * d'OpenGL.
 *
 */
 
typedef struct Mat44f Mat44f;

struct Mat44f
{
	float e11;
	float e21;
	float e31;
	float e41;
	float e12;
	float e22;
	float e32;
	float e42;
	float e13;
	float e23;
	float e33;
	float e43;
	float e14;
	float e24;
	float e34;
	float e44;
};

void Mat44f_SetLine1(Mat44f* This, float e11, float e12, float e13, float e14);
void Mat44f_SetLine2(Mat44f* This, float e21, float e22, float e23, float e24);
void Mat44f_SetLine3(Mat44f* This, float e31, float e32, float e33, float e34);
void Mat44f_SetLine4(Mat44f* This, float e41, float e42, float e43, float e44);
void Mat44f_Clear(Mat44f* This);
void Mat44f_Add(Mat44f* This, Mat44f* Other);
void Mat44f_Subtract(Mat44f* This, Mat44f* Other);
void Mat44f_Multiply(Mat44f* This, Mat44f* Other);
void Mat44f_ProductByScalar(Mat44f* This, float Scalar);
void Mat44f_ProductMatrixVector(Mat44f* This, Vec4f* Vector, Vec4f* NewVector);
void Mat44f_Transpose(Mat44f* This);
void Mat44f_Identity(Mat44f* This);
float Mat44f_Determinant(Mat44f* This);
int Mat44f_Inverse(Mat44f* This, Mat44f* Inverse);
void Mat44f_Translation(Mat44f* This, Vec3f* Vector);
void Mat44f_TranslationEx(Mat44f* This, float Tx, float Ty, float Tz);
void Mat44f_Scale(Mat44f* This, Vec3f* Vector);
void Mat44f_ScaleEx(Mat44f* This, float Sx, float Sy, float Sz);
void Mat44f_RotateX(Mat44f* This, float Theta);
void Mat44f_RotateY(Mat44f* This, float Theta);
void Mat44f_RotateZ(Mat44f* This, float Theta);
void Mat44f_Perspective(Mat44f* This, float FOVY, float Aspect, float Near, float Far);
void Mat44f_Orthogonal(Mat44f* This, float Left, float Right, float Bottom, float Top, float Near, float Far);
void Mat44f_ApplyTransformation(Mat44f* This, Vec3f* Vector, Vec3f* NewVector, int Mode);

#if defined(USE_MAT_44F_AS_GENERIC_ITEM)
    void Mat44f_GenericItem_Init(void*);
    void Mat44f_GenericItem_Set(void*, void*);
    void Mat44f_GenericItem_Swap(void*, void*);
#endif

#endif

