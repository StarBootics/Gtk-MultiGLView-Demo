/*
 * Quat.c
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
#include "Quat.h"

void Quat_Add(Quat* This, Quat* Other)
{
	This->S += Other->S;
	This->I += Other->I;
	This->J += Other->J;
	This->K += Other->K;
}

void Quat_Subtract(Quat* This, Quat* Other)
{
	This->S -= Other->S;
	This->I -= Other->I;
	This->J -= Other->J;
	This->K -= Other->K;
}

void Quat_Product(Quat* This, Quat* Other, Quat* Result)
{
	float S = This->S * Other->S - This->I * Other->I - This->J * Other->J - This->K * Other->K;
	float I = This->S * Other->I + This->I * Other->S + This->J * Other->K - This->K * Other->J;
	float J = This->S * Other->J - This->I * Other->K + This->J * Other->S + This->K * Other->I;
	float K = This->S * Other->K + This->I * Other->J - This->J * Other->I + This->K * Other->S;
	
	Result->S = S;
	Result->I = I;
	Result->J = J;
	Result->K = K;
	
}

void Quat_ProductVector(Quat* This, Vec3f* Vector, Vec3f* Result)
{
	float ProdX = This->S * Vector->X + This->I * Vector->Z - This->K * Vector->Y;
	float ProdY = This->S * Vector->Y + This->J * Vector->X - This->I * Vector->Z;
	float ProdZ = This->S * Vector->Z + This->K * Vector->Y - This->J * Vector->X;
	float ProdW = -This->I * Vector->X - This->J * Vector->Y - This->K * Vector->Z;
	
	Result->X = This->S * ProdX - ProdY * This->K + ProdZ * This->J - ProdW * This->I;
	Result->Y = This->S * ProdY - ProdZ * This->I + ProdX * This->K - ProdW * This->J;
	Result->Z = This->S * ProdZ - ProdX * This->J + ProdY * This->I - ProdW * This->K;
	
}

void Quat_Multiply(Quat* This, Quat* Other)
{
	float S = This->S * Other->S - This->I * Other->I - This->J * Other->J - This->K * Other->K;
	float I = This->S * Other->I + This->I * Other->S + This->J * Other->K - This->K * Other->J;
	float J = This->S * Other->J - This->I * Other->K + This->J * Other->S + This->K * Other->I;
	float K = This->S * Other->K + This->I * Other->J - This->J * Other->I + This->K * Other->S;
		
	This->S = S;
	This->I = I;	
	This->J = J;
	This->K = K; 
}

void Quat_Inverse(Quat* This)
{
	This->I *= -1.0f;
	This->J *= -1.0f;
	This->K *= -1.0f;
}


float Quat_SquareModulus(Quat* This)
{
	return This->S * This->S + This->I * This->I + This->J * This->J + This->K * This->K;
}

float Quat_Modulus(Quat* This)
{
	return sqrtf(This->S * This->S + This->I * This->I + This->J * This->J + This->K * This->K);
}

int Quat_Compare(Quat* This, Quat* Other, float Epsilon)
{
	return (fabsf(This->S - Other->S) < Epsilon && fabsf(This->I - Other->I) < Epsilon && fabsf(This->J - Other->J) < Epsilon && fabsf(This->K - Other->K) < Epsilon);
}

int Quat_IsFinite(Quat* This)
{
	return (isfinite(This->S) && isfinite(This->I) && isfinite(This->J) && isfinite(This->K));
}

int Quat_IsUnit(Quat* This)
{
	float Modulus = Quat_Modulus(This);
	return (fabsf(Modulus - 1.0f) < 0.00001);
}

int Quat_IsValid(Quat* This)
{
	return (Quat_IsFinite(This) && Quat_IsUnit(This));
}

void Quat_Normalize(Quat* This)
{
		float Modulus = Quat_Modulus(This);
		
		if (Modulus == 0.0)
		{
			This->S = 1.0;
			This->I = 0.0;
			This->J = 0.0;
			This->K = 0.0;
		}
		else
		{
			float InvModulus = 1.0 / Modulus;
			This->S *= InvModulus;
			This->I *= InvModulus;
			This->J *= InvModulus;
			This->K *= InvModulus;
		}
}

void Quat_DirectRotation(Quat* This, Vec3f* NewVector, Vec3f* Vector)
{
	float ix = This->S * Vector->X + This->J * Vector->Z - This->K * Vector->Y;
	float iy = This->S * Vector->Y + This->K * Vector->X - This->I * Vector->Z;
	float iz = This->S * Vector->Z + This->I * Vector->Y - This->J * Vector->X;
	float is = -This->I * Vector->X - This->J * Vector->Y - This->K * Vector->Z;
		
	NewVector->X = ix * This->S + is * -This->I + iy * -This->K - iz * -This->J;
	NewVector->Y = iy * This->S + is * -This->J + iz * -This->I - ix * -This->K;
	NewVector->Z = iz * This->S + is * -This->K + ix * -This->J - iy * -This->I;
}

void Quat_Lerp(Quat* This, Quat* Other, float Proportion, Quat* Destination)
{
	Destination->S = This->S + Proportion * (Other->S - This->S);
	Destination->I = This->I + Proportion * (Other->I - This->I);
	Destination->J = This->J + Proportion * (Other->J - This->J);
	Destination->K = This->K + Proportion * (Other->K - This->K);
	Quat_Normalize(Destination);
}

void Quat_Slerp(Quat* This, Quat* Other, float Proportion, Quat* Destination)
{
	float Dot = This->S * Other->S + This->I * Other->I + This->J * Other->J + This->K * Other->K;
		
	if (Dot > 0.9995)
	{
		Destination->S = This->S + Proportion * (Other->S - This->S);
		Destination->I = This->I + Proportion * (Other->I - This->I);
		Destination->J = This->J + Proportion * (Other->J - This->J);
		Destination->K = This->K + Proportion * (Other->K - This->K);
	}  
	else
	{
		float One_Minus_t = 1.0 - Proportion;
		float Theta = acosf(Dot);
		float SinTheta = sinf(Theta);
		float Wa = sinf(One_Minus_t * Theta) / SinTheta;
		float Wb = sinf(Proportion * Theta) / SinTheta;
		
		Destination->S = This->S * Wa + Other->S * Wb;
		Destination->I = This->I * Wa + Other->I * Wb;
		Destination->J = This->J * Wa + Other->J * Wb;
		Destination->K = This->K * Wa + Other->K * Wb;
	}  
		
	Quat_Normalize(Destination);
}

void Quat_FromAxisAngle(Quat* This, Vec3f* Axis, float Angle)
{
	float HalfAngle = Angle * 0.5;
	float SinHalfAngle = sinf(HalfAngle);
		
	This->S = cosf(HalfAngle);
	This->I = Axis->X * SinHalfAngle;
	This->J = Axis->Y * SinHalfAngle;
	This->K = Axis->Z * SinHalfAngle;
	
	Quat_Normalize(This);
	
}

void Quat_FromVectors(Quat* This, Vec3f* V1, Vec3f* V2)
{
	float Epsilon = 0.001f;
	
	Vec3f RV2 = (Vec3f){-V2->X, -V2->Y, -V2->Z};
	Vec3f Axis = (Vec3f){0.0f, 0.0f, 0.0f};
		
	if (Vec3f_Compare(V1, V2, Epsilon))
	{
		Quat_FromAxisAngle(This, V1, 0.0f);
		return;
	}
	else if (Vec3f_Compare(V1, &RV2, Epsilon))
	{
		if (V1->X > -Epsilon && V1->X < Epsilon)
		{
			Axis = (Vec3f){1.0f, 0.0f, 0.0f};
		}
		else if (V1->Y > -Epsilon && V1->Y < Epsilon)
		{  
			Axis = (Vec3f){0.0f, 1.0f, 0.0f};
		}
		else if (V1->Z > -Epsilon && V1->Z < Epsilon)
		{  
			Axis = (Vec3f){0.0f, 0.0f, 1.0f};
		}
		  
		Quat_FromAxisAngle(This, &Axis, M_PI);
		return;
	}
	
	Vec3f U1 = (Vec3f){V1->X, V1->Y, V1->Z};
	Vec3f U2 = (Vec3f){V2->X, V2->Y, V2->Z};

	Vec3f_Normalize(&U1);
	Vec3f_Normalize(&U2);
	
	Vec3f_Cross(&U1, &U2, &Axis);
	Vec3f_Normalize(&Axis);
	Quat_FromAxisAngle(This, &Axis, acosf(Vec3f_Dot(&U1, &U2)));
}

void Quat_ToRotationMatrix(Quat* This, Mat44f* Rotation)
{
	Quat_Normalize(This);
	
	float II = This->I * This->I;
	float JJ = This->J * This->J;
	float KK = This->K * This->K;
	
	float IJ = This->I * This->J;
	float IK = This->I * This->K;
	float JK = This->J * This->K;
	
	float SI = This->S * This->I;
	float SJ = This->S * This->J;
	float SK = This->S * This->K;
	
	Mat44f_SetLine1(Rotation, 1.0f - 2.0f * (JJ + KK), 2.0f * (IJ - SK), 2.0f * (IK + SJ), 0.0f);
	Mat44f_SetLine2(Rotation, 2.0f * (IJ + SK), 1.0f - 2.0f * (II + KK), 2.0f * (JK - SI), 0.0f);
	Mat44f_SetLine3(Rotation, 2.0f * (IK - SJ), 2.0f * (JK + SI), 1.0f - 2.0f * (II + JJ), 0.0f);
	Mat44f_SetLine4(Rotation, 0.0f, 0.0f, 0.0f, 1.0f);
}

void Quat_FromRotationMatrix(Quat* This, Mat44f* Rotation)
{
	float E11 = Rotation->e11;
	float E22 = Rotation->e22;
	float E33 = Rotation->e33;
	
	float Trace = E11 + E22 + E33;
	
    if (Trace > 0.0f) 
    {
        float s = sqrtf(Trace + 1.0f) * 2.0f; // s = 4 * qw
        This->S = 0.25f * s;
        This->I = (Rotation->e32 - Rotation->e23) / s;
        This->J = (Rotation->e13 - Rotation->e31) / s;
        This->K = (Rotation->e21 - Rotation->e12) / s;
    } 
    else if ((Rotation->e11 > Rotation->e22) && (Rotation->e11 > Rotation->e33)) 
    {
        float s = sqrtf(1.0f + Rotation->e11 - Rotation->e22 - Rotation->e33) * 2.0f; // s = 4 * qx
        This->S = (Rotation->e32 - Rotation->e23) / s;
        This->I = 0.25f * s;
        This->J = (Rotation->e12 + Rotation->e21) / s;
        This->K = (Rotation->e13 + Rotation->e31) / s;
    } 
    else if (Rotation->e22 > Rotation->e33) 
    {
        float s = sqrtf(1.0f + Rotation->e22 - Rotation->e11 - Rotation->e33) * 2.0f; // s = 4 * qy
        This->S = (Rotation->e13 - Rotation->e31) / s;
        This->I = (Rotation->e12 + Rotation->e21) / s;
        This->J = 0.25f * s;
        This->K = (Rotation->e23 + Rotation->e32) / s;
    } 
    else 
    {
        float s = sqrtf(1.0f + Rotation->e33 - Rotation->e11 - Rotation->e22) * 2.0f; // s = 4 * qz
        This->S = (Rotation->e21 - Rotation->e12) / s;
        This->I = (Rotation->e13 + Rotation->e31) / s;
        This->J = (Rotation->e23 + Rotation->e32) / s;
        This->K = 0.25f * s;
    }
	
	Quat_Normalize(This);
	
}

void Quat_FromEulerAngles(Quat* This, Vec3f* Angles)
{
	float Angle = Angles->X * 0.5f;
	float c1 = cosf(Angle);
	float s1 = sinf(Angle);
	
	Angle = Angles->Y * 0.5f;
	float c2 = cosf(Angle);
	float s2 = sinf(Angle);
	
	Angle = Angles->Z * 0.5f;
	float c3 = cosf(Angle);
	float s3 = sinf(Angle);
	
	This->S = c1 * c2 * c3 - s1 * s2 * s3;
	This->I = s1 * c2 * c3 + c1 * s2 * s3;
	This->J = c1 * s2 * c3 - s1 * c2 * s3;
	This->K = c1 * c2 * s3 + s1 * s2 * c3;
	
	Quat_Normalize(This);
}

