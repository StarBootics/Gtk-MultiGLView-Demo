/*
 * Mat44f.c
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

#include "Mat44f.h"

void Mat44f_SetLine1(Mat44f* This, float e11, float e12, float e13, float e14)
{
	This->e11 = e11;
	This->e12 = e12;
	This->e13 = e13;
	This->e14 = e14;
}

void Mat44f_SetLine2(Mat44f* This, float e21, float e22, float e23, float e24)
{
	This->e21 = e21;
	This->e22 = e22;
	This->e23 = e23;
	This->e24 = e24;
}

void Mat44f_SetLine3(Mat44f* This, float e31, float e32, float e33, float e34)
{
	This->e31 = e31;
	This->e32 = e32;
	This->e33 = e33;
	This->e34 = e34;
}

void Mat44f_SetLine4(Mat44f* This, float e41, float e42, float e43, float e44)
{
	This->e41 = e41;
	This->e42 = e42;
	This->e43 = e43;
	This->e44 = e44;
}

void Mat44f_Clear(Mat44f* This)
{
	This->e11 = 0.0f;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 0.0f;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = 0.0f;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 0.0f;
}

void Mat44f_Add(Mat44f* This, Mat44f* Other)
{
	This->e11 += Other->e11;
	This->e21 += Other->e21;
	This->e31 += Other->e31;
	This->e41 += Other->e41;
	This->e12 += Other->e12;
	This->e22 += Other->e22;
	This->e32 += Other->e32;
	This->e42 += Other->e42;
	This->e13 += Other->e13;
	This->e23 += Other->e23;
	This->e33 += Other->e33;
	This->e43 += Other->e43;
	This->e14 += Other->e14;
	This->e24 += Other->e24;
	This->e34 += Other->e34;
	This->e44 += Other->e44;
}

void Mat44f_Subtract(Mat44f* This, Mat44f* Other)
{
	This->e11 -= Other->e11;
	This->e21 -= Other->e21;
	This->e31 -= Other->e31;
	This->e41 -= Other->e41;
	This->e12 -= Other->e12;
	This->e22 -= Other->e22;
	This->e32 -= Other->e32;
	This->e42 -= Other->e42;
	This->e13 -= Other->e13;
	This->e23 -= Other->e23;
	This->e33 -= Other->e33;
	This->e43 -= Other->e43;
	This->e14 -= Other->e14;
	This->e24 -= Other->e24;
	This->e34 -= Other->e34;
	This->e44 -= Other->e44;
}

void Mat44f_Multiply(Mat44f* This, Mat44f* Other)
{
	float e11 = This->e11 * Other->e11 + This->e12 * Other->e21 + This->e13 * Other->e31 + This->e14 * Other->e41;
	float e12 = This->e11 * Other->e12 + This->e12 * Other->e22 + This->e13 * Other->e32 + This->e14 * Other->e42;
	float e13 = This->e11 * Other->e13 + This->e12 * Other->e23 + This->e13 * Other->e33 + This->e14 * Other->e43;
	float e14 = This->e11 * Other->e14 + This->e12 * Other->e24 + This->e13 * Other->e34 + This->e14 * Other->e44;
	float e21 = This->e21 * Other->e11 + This->e22 * Other->e21 + This->e23 * Other->e31 + This->e24 * Other->e41;
	float e22 = This->e21 * Other->e12 + This->e22 * Other->e22 + This->e23 * Other->e32 + This->e24 * Other->e42;
	float e23 = This->e21 * Other->e13 + This->e22 * Other->e23 + This->e23 * Other->e33 + This->e24 * Other->e43;
	float e24 = This->e21 * Other->e14 + This->e22 * Other->e24 + This->e23 * Other->e34 + This->e24 * Other->e44;
	float e31 = This->e31 * Other->e11 + This->e32 * Other->e21 + This->e33 * Other->e31 + This->e34 * Other->e41;
	float e32 = This->e31 * Other->e12 + This->e32 * Other->e22 + This->e33 * Other->e32 + This->e34 * Other->e42;
	float e33 = This->e31 * Other->e13 + This->e32 * Other->e23 + This->e33 * Other->e33 + This->e34 * Other->e43;
	float e34 = This->e31 * Other->e14 + This->e32 * Other->e24 + This->e33 * Other->e34 + This->e34 * Other->e44;
	float e41 = This->e41 * Other->e11 + This->e42 * Other->e21 + This->e43 * Other->e31 + This->e44 * Other->e41;
	float e42 = This->e41 * Other->e12 + This->e42 * Other->e22 + This->e43 * Other->e32 + This->e44 * Other->e42;
	float e43 = This->e41 * Other->e13 + This->e42 * Other->e23 + This->e43 * Other->e33 + This->e44 * Other->e43;
	float e44 = This->e41 * Other->e14 + This->e42 * Other->e24 + This->e43 * Other->e34 + This->e44 * Other->e44;
			
	This->e11 = e11;
	This->e12 = e12;
	This->e13 = e13;
	This->e14 = e14;
			
	This->e21 = e21;
	This->e22 = e22;
	This->e23 = e23;
	This->e24 = e24;
			
	This->e31 = e31;
	This->e32 = e32;
	This->e33 = e33;
	This->e34 = e34;
			
	This->e41 = e41;
	This->e42 = e42;
	This->e43 = e43;
	This->e44 = e44;
}

void Mat44f_ProductByScalar(Mat44f* This, float Scalar)
{
	This->e11 *= Scalar;
	This->e12 *= Scalar;
	This->e13 *= Scalar;
	This->e14 *= Scalar;
			
	This->e21 *= Scalar;
	This->e22 *= Scalar;
	This->e23 *= Scalar;
	This->e24 *= Scalar;
			
	This->e31 *= Scalar;
	This->e32 *= Scalar;
	This->e33 *= Scalar;
	This->e34 *= Scalar;
			
	This->e41 *= Scalar;
	This->e42 *= Scalar;
	This->e43 *= Scalar;
	This->e44 *= Scalar;
}

void Mat44f_ProductMatrixVector(Mat44f* This, Vec4f* Vector, Vec4f* NewVector)
{

	float X = This->e11 * Vector->X + This->e12 * Vector->Y + This->e13 * Vector->Z + This->e14 * Vector->W;
	float Y = This->e21 * Vector->X + This->e22 * Vector->Y + This->e23 * Vector->Z + This->e24 * Vector->W;
	float Z = This->e31 * Vector->X + This->e32 * Vector->Y + This->e33 * Vector->Z + This->e34 * Vector->W;
	float W = This->e41 * Vector->X + This->e42 * Vector->Y + This->e43 * Vector->Z + This->e44 * Vector->W;
	
	NewVector->X = X;
	NewVector->Y = Y;
	NewVector->Z = Z;
	NewVector->W = W;
}

void Mat44f_Transpose(Mat44f* This)
{
	float Temp = This->e21;
	This->e21 = This->e12;
	This->e12 = Temp;
	
	Temp = This->e31;
	This->e31 = This->e13;
	This->e13 = Temp;
	
	Temp = This->e41;
	This->e41 = This->e14;
	This->e14 = Temp;
	
	Temp = This->e32;
	This->e32 = This->e23;
	This->e23 = Temp;
	
	Temp = This->e42;
	This->e42 = This->e24;
	This->e24 = Temp;
	
	Temp = This->e43;
	This->e43 = This->e34;
	This->e34 = Temp;
}

void Mat44f_Identity(Mat44f* This)
{
	This->e11 = 1.0f;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 1.0f;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = 1.0f;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

#define Cofactor(m0, m1, m2, m3, m4, m5, m6, m7, m8) (m0 * (m4 * m8 - m5 * m7) - m1 * (m3 * m8 - m5 * m6) + m2 * (m3 * m7 - m4 * m6))

float Mat44f_Determinant(Mat44f* This)
{
	float Temp00 = This->e11 * Cofactor(This->e22, This->e32, This->e42, This->e23, This->e33, This->e43, This->e24, This->e34, This->e44);
	float Temp01 = This->e21 * Cofactor(This->e12, This->e32, This->e42, This->e13, This->e33, This->e43, This->e14, This->e34, This->e44);
	float Temp02 = This->e31 * Cofactor(This->e12, This->e22, This->e42, This->e13, This->e23, This->e43, This->e14, This->e24, This->e44);
	float Temp03 = This->e41 * Cofactor(This->e12, This->e22, This->e32, This->e13, This->e23, This->e33, This->e14, This->e24, This->e34);
    
	return Temp00 - Temp01 + Temp02 - Temp03;
}

int Mat44f_Inverse(Mat44f* This, Mat44f* Inverse)
{
	float Cofactor00 = Cofactor(This->e22, This->e32, This->e42, This->e23, This->e33, This->e43, This->e24, This->e34, This->e44);
	float Cofactor01 = Cofactor(This->e12, This->e32, This->e42, This->e13, This->e33, This->e43, This->e14, This->e34, This->e44);
	float Cofactor02 = Cofactor(This->e12, This->e22, This->e42, This->e13, This->e23, This->e43, This->e14, This->e24, This->e44);
	float Cofactor03 = Cofactor(This->e12, This->e22, This->e32, This->e13, This->e23, This->e33, This->e14, This->e24, This->e34);
		
	float Determinant = This->e11 * Cofactor00 - This->e21 * Cofactor01 + This->e31 * Cofactor02 - This->e41 * Cofactor03;
		
	if (fabsf(Determinant) <= 0.00001f)
	{
		Mat44f_Identity(Inverse);
		return 0;
	}
		
	float Cofactor04 = Cofactor(This->e21, This->e31, This->e41, This->e23, This->e33, This->e43, This->e24, This->e34, This->e44);
	float Cofactor05 = Cofactor(This->e11, This->e31, This->e41, This->e13, This->e33, This->e43, This->e14, This->e34, This->e44);
	float Cofactor06 = Cofactor(This->e11, This->e21, This->e41, This->e13, This->e23, This->e43, This->e14, This->e24, This->e44);
	float Cofactor07 = Cofactor(This->e11, This->e21, This->e31, This->e13, This->e23, This->e33, This->e14, This->e24, This->e34);
		
	float Cofactor08 = Cofactor(This->e21, This->e31, This->e41, This->e22, This->e32, This->e42, This->e24, This->e34, This->e44);
	float Cofactor09 = Cofactor(This->e11, This->e31, This->e41, This->e12, This->e32, This->e42, This->e14, This->e34, This->e44);
	float Cofactor10 = Cofactor(This->e11, This->e21, This->e41, This->e12, This->e22, This->e42, This->e14, This->e24, This->e44);
	float Cofactor11 = Cofactor(This->e11, This->e21, This->e31, This->e12, This->e22, This->e32, This->e14, This->e24, This->e34);
	
	float Cofactor12 = Cofactor(This->e21, This->e31, This->e41, This->e22, This->e32, This->e42, This->e23, This->e33, This->e43);
	float Cofactor13 = Cofactor(This->e11, This->e31, This->e41, This->e12, This->e32, This->e42, This->e13, This->e33, This->e43);
	float Cofactor14 = Cofactor(This->e11, This->e21, This->e41, This->e12, This->e22, This->e42, This->e13, This->e23, This->e43);
	float Cofactor15 = Cofactor(This->e11, This->e21, This->e31, This->e12, This->e22, This->e32, This->e13, This->e23, This->e33);
	
	float InvDeterminant = 1.0 / Determinant;
		
	Inverse->e11 = InvDeterminant * Cofactor00;
	Inverse->e21 = -InvDeterminant * Cofactor04;
	Inverse->e31 = InvDeterminant * Cofactor08;
	Inverse->e41 = -InvDeterminant * Cofactor12;
		
	Inverse->e12 = -InvDeterminant * Cofactor01;
	Inverse->e22 = InvDeterminant * Cofactor05;
	Inverse->e32 = -InvDeterminant * Cofactor09;
	Inverse->e42 = InvDeterminant * Cofactor13;
		
	Inverse->e13 = InvDeterminant * Cofactor02;
	Inverse->e23 = -InvDeterminant * Cofactor06;
	Inverse->e33 = InvDeterminant * Cofactor10;
	Inverse->e43 = -InvDeterminant * Cofactor14;
	
	Inverse->e14 = -InvDeterminant * Cofactor03;
	Inverse->e24 = InvDeterminant * Cofactor07;
	Inverse->e34 = -InvDeterminant * Cofactor11;
	Inverse->e44 = InvDeterminant * Cofactor15;
    
	return 1;
}

void Mat44f_Translation(Mat44f* This, Vec3f* Vector)
{
	This->e11 = 1.0f;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 1.0f;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = 1.0f;
	This->e43 = 0.0f;
	This->e14 = Vector->X;
	This->e24 = Vector->Y;
	This->e34 = Vector->Z;
	This->e44 = 1.0f;
}

void Mat44f_TranslationEx(Mat44f* This, float Tx, float Ty, float Tz)
{
	This->e11 = 1.0f;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 1.0f;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = 1.0f;
	This->e43 = 0.0f;
	This->e14 = Tx;
	This->e24 = Ty;
	This->e34 = Tz;
	This->e44 = 1.0f;
}

void Mat44f_Scale(Mat44f* This, Vec3f* Vector)
{
	This->e11 = Vector->X;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = Vector->Y;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = Vector->Z;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

void Mat44f_ScaleEx(Mat44f* This, float Sx, float Sy, float Sz)
{
	This->e11 = Sx;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = Sy;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = Sz;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

void Mat44f_RotateX(Mat44f* This, float Theta)
{
	float CosTheta = cosf(Theta);
	float SinTheta = sinf(Theta);

	This->e11 = 1.0f;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = CosTheta;
	This->e32 = SinTheta;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = -SinTheta;
	This->e33 = CosTheta;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

void Mat44f_RotateY(Mat44f* This, float Theta)
{
	float CosTheta = cosf(Theta);
	float SinTheta = sinf(Theta);

	This->e11 = CosTheta;
	This->e21 = 0.0f;
	This->e31 = -SinTheta;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 1.0f;
	This->e32 = 0.0;
	This->e42 = 0.0f;
	This->e13 = SinTheta;
	This->e23 = 0.0f;
	This->e33 = CosTheta;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

void Mat44f_RotateZ(Mat44f* This, float Theta)
{
	float CosTheta = cosf(Theta);
	float SinTheta = sinf(Theta);

	This->e11 = CosTheta;
	This->e21 = SinTheta;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = -SinTheta;
	This->e22 = CosTheta;
	This->e32 = 0.0;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = 1.0f;
	This->e43 = 0.0f;
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = 0.0f;
	This->e44 = 1.0f;
}

void Mat44f_Perspective(Mat44f* This, float FOVY, float Aspect, float Near, float Far)
{
	float Sy = 1.0f / tanf(FOVY/2.0f);
	float Sx = Sy / Aspect;
	float Sz = (Far + Near) / (Near - Far);
	float Pz = (2.0f * Far * Near) / (Near - Far);
		
	This->e11 = Sx;
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	
	This->e12 = 0.0f;
	This->e22 = Sy;
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = Sz;
	This->e43 = -1.0f;
	
	This->e14 = 0.0f;
	This->e24 = 0.0f;
	This->e34 = Pz;
	This->e44 = 0.0f;
}

void Mat44f_Orthogonal(Mat44f* This, float Left, float Right, float Bottom, float Top, float Near, float Far)
{
	This->e11 = 2.0f / (Right - Left);
	This->e21 = 0.0f;
	This->e31 = 0.0f;
	This->e41 = 0.0f;
	This->e12 = 0.0f;
	This->e22 = 2.0f / (Top - Bottom);
	This->e32 = 0.0f;
	This->e42 = 0.0f;
	This->e13 = 0.0f;
	This->e23 = 0.0f;
	This->e33 = -2.0f / (Far - Near);
	This->e43 = 0.0f;
	This->e14 = -((Right + Left) / (Right - Left));
	This->e24 = -((Top + Bottom) / (Top - Bottom));
	This->e34 = -((Far + Near) / (Far - Near));
	This->e44 = 1.0f;
}

void Mat44f_ApplyTransformation(Mat44f* This, Vec3f* Vector, Vec3f* NewVector, int Mode)
{
	Vec4f HomoVector = {Vector->X, Vector->Y, Vector->Z, 1.0f};
	Vec4f NewHomoVector = {0.0f, 0.0f, 0.0f, 0.0f};
	
	if (Mode == 1)
	{
		HomoVector.W = 0.0f;
	}
	
	Mat44f_ProductMatrixVector(This, &HomoVector, &NewHomoVector);
	
	NewVector->X = NewHomoVector.X;
	NewVector->Y = NewHomoVector.Y;
	NewVector->Z = NewHomoVector.Z;
	
	if (Mode == 1)
	{
		Vec3f_Normalize(NewVector);
	}
	
}

#if defined(USE_MAT_44F_AS_GENERIC_ITEM)

	void Mat44f_GenericItem_Init(void* VectorA)
	{
		Mat44f_Clear((Mat44f*) VectorA);
	}

	void Mat44f_GenericItem_Set(void* VectorA, void* VectorB)
	{
		*(Mat44f*)VectorA = *(Mat44f*)VectorB;
	}

	void Mat44f_GenericItem_Swap(void* VectorA, void* VectorB)
	{
		Mat44f temp = *(Mat44f*)VectorA;
		*(Mat44f*)VectorA = *(Mat44f*)VectorB;
		*(Mat44f*)VectorB = temp;
	}

#endif

