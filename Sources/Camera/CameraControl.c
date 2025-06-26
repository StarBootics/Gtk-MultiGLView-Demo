/*
 * CameraControl.c
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
 */

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// This CameraControl is highly inspired by Song Ho Ahn. In fact it's the
// result of fusion of the "OrbitCamera" and the "TrackBall" source codes
// into this Camera control system.
//
// The original source codes can be download from this website :
// https://songho.ca/opengl/gl_camera.html
//
// From the Author himself about source code re-use :
//
// There is no license for my OrbitCamera and other C++ code. You can 
// use/modify/distribute it without any restriction. However, it would 
// be great if you mention where you reference from. That is all.
//
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include <stddef.h>
#include <math.h>

#include "CameraControl.h"

static void CameraControl_ComputeVectorProjection(CameraControl* This, float x, float y, Vec3f* Vector)
{
	Vector->X = x;
	Vector->Y = y;
	Vector->Z = 0.0f;
	
	float d = x * x + y * y;
	float rr = This->Radius * This->Radius;
	
	if (d <= (rr * 0.5f))
	{
		Vector->Z = sqrtf(rr - d);
	}
	else
	{
		Vector->Z = 0.5f * rr / sqrtf(d);
		float x2, y2;
		
		if (x == 0.0f)
		{
			x2 = 0.0f;
			y2 = sqrtf(rr - Vector->Z * Vector->Z);
			
			if (y < 0.0f)
			{
				y2 = -y2;
			}
			
		}
		else
		{
			float a = y / x;
			
			x2 = sqrtf((rr - Vector->Z * Vector->Z) / (1.0f + a * a));
			
			if (x < 0.0f)
			{
				x2 = -x2;
			}
			
			y2 = a * x2;
		}
		
		Vector->X = x2;
		Vector->Y = y2;
	}
}

static void CameraControl_ComputeVectorArc(CameraControl* This, float x, float y, Vec3f* Vector)
{
	float Arc = sqrtf(x * x + y * y);
	float a = Arc / This->Radius;
	float b = atan2f(y, x);
	float x2 = This->Radius * sinf(a);
	
	Vector->X = x2 * cosf(b);
	Vector->Y = x2 * sinf(b);
	Vector->Z = This->Radius * cosf(a);
}

static void CameraControl_ComputeVector(CameraControl* This, float x, float y, Vec3f* Vector)
{
	if (This->Radius == 0.0f || This->ViewWidth == 0 || This->ViewHeight == 0)
	{
		*(Vector) = (Vec3f) {0.0f, 0.0f, 0.0f};
		return;
	}
	
	float Mx = x - This->HalfViewWidth;
	float My = This->HalfViewHeight - y;
	
	if (This->Settings->GetTrackBallMode(This->Settings) == TRACK_BALL_MODE_PROJECT)
	{
		CameraControl_ComputeVectorProjection(This, Mx, My, Vector);
	}
	else
	{
		CameraControl_ComputeVectorArc(This, Mx, My, Vector);
	}
	
	Vec3f_Normalize(Vector);
	
}

static void CameraControl_ComputeStuff(Vec3f* Target, float Distance, Quat* Orientation, Mat44f* ViewMatrix, Vec3f* Position)
{
	Vec3f InvTarget = (Vec3f) {Target->X * -1.0f, Target->Y * -1.0f, Target->Z * -1.0f};
	
	Mat44f RotMat, TransMat, DistMat, Concat;
	Mat44f_Identity(&TransMat);
	Mat44f_Identity(&RotMat);
	Mat44f_Identity(&DistMat);
	Mat44f_Identity(&Concat);
	
	Quat_ToRotationMatrix(Orientation, &RotMat);
	Mat44f_Translation(&TransMat, &InvTarget);
	Mat44f_TranslationEx(&DistMat, 0.0f, 0.0f, -Distance);
	
	Mat44f_Multiply(&Concat, &DistMat);
	Mat44f_Multiply(&Concat, &RotMat);
	Mat44f_Multiply(&Concat, &TransMat);
	
	if (ViewMatrix != NULL)
	{
		*(ViewMatrix) = Concat;
	}
	
	if (Position != NULL)
	{
		Vec3f Forward = (Vec3f) {-Concat.e31, -Concat.e32, -Concat.e33};
		Vec3f_ProductByScalar(&Forward, Distance);
		*(Position) = *(Target);
		Vec3f_Subtract(Position, &Forward);
	}	
}

CameraControlMode CameraControl_GetMode(CameraControl* This)
{
	return This->Mode;
}

Mat44f* CameraControl_GetViewMatrix(CameraControl* This)
{
	return &This->ViewMatrix;
}

Mat44f* CameraControl_GetInvViewMatrix(CameraControl* This)
{
	return &This->InvViewMatrix;
}

void CameraControl_SetMode(CameraControl* This, CameraControlMode Mode)
{
	This->Mode = Mode;
}

void CameraControl_SetRadius(CameraControl* This, float Radius)
{
	This->Radius = Radius;
}
void CameraControl_SetViewWidth(CameraControl* This, int Width)
{
	This->ViewWidth = Width;
	This->HalfViewWidth = ((float) Width) * 0.5f;
}

void CameraControl_SetViewHeight(CameraControl* This, int Height)
{
	This->ViewHeight = Height;
	This->HalfViewHeight = ((float) Height) * 0.5f;
}

void CameraControl_MoveTo(CameraControl* This, Vec3f* To)
{
	if (This->Animation == CAMERA_CONTROL_ANIMATION_ACTIVE)
	{
		This->MovingFrom = This->Position;
		This->MovingTo = *(To);
		This->MovingTime = 0.0f;
		This->IsMoving = TRUE;
	}
	else
	{
		This->Position = *(To);
	}
}

void CameraControl_ShiftTo(CameraControl* This, Vec3f* To)
{
	if (This->Animation == CAMERA_CONTROL_ANIMATION_ACTIVE)
	{
		This->ShiftingFrom = This->Target;
		This->ShiftingTo =  *(To);
		This->ShiftingTime = 0.0f;
		This->IsShifting = TRUE;
	}
	else
	{
		This->Target = *(To);
	}
}

void CameraControl_RotateTo(CameraControl* This, Quat* To)
{
	if (This->Animation == CAMERA_CONTROL_ANIMATION_ACTIVE)
	{
		This->TurningFrom = This->Orientation;
		This->TurningTo = *(To);
		This->TurningTime = 0.0f;
		This->IsTurning = TRUE;
	}
	else
	{
		This->Orientation = *(To);
	}
}

void CameraControl_ForwardTo(CameraControl* This, float NewDistance)
{
	if (This->Animation == CAMERA_CONTROL_ANIMATION_ACTIVE)
	{
		This->ForwardingFrom = This->Distance;
		This->ForwardingTo = NewDistance;
		This->ForwardingTime = 0.0f;
		This->IsForwarding = TRUE;
	}
	else
	{
		This->Distance -= NewDistance;
		
		if (This->Distance < This->MinDistance)
		{
			This->Distance = This->MinDistance;
		}
	
	}
}

void CameraControl_RestoreToPerspectiveView(CameraControl* This)
{
	Vec3f LookInDirection = (Vec3f) { -5.0f, -4.0f, -5.0f};
	Vec3f_Normalize(&LookInDirection);
	Vec3f XAxis = (Vec3f) {1.0f, 0.0f, 0.0f};
	Vec3f YAxis = (Vec3f) {0.0f, 1.0f, 0.0f};
	
	float HorizontalAngle = atan2(LookInDirection.X, LookInDirection.Z) + M_PI / 2.0f;
	float VerticalAngle = -asinf(LookInDirection.Y);
	
	Quat HorizontalRotation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	Quat_FromAxisAngle(&HorizontalRotation, &YAxis, HorizontalAngle);
	
	Quat VerticalRotation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	Quat_FromAxisAngle(&VerticalRotation, &XAxis, VerticalAngle);
	
	Quat TotalRotation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};	
	
	Quat_Multiply(&TotalRotation, &VerticalRotation);
	Quat_Multiply(&TotalRotation, &HorizontalRotation);
	Quat_Normalize(&TotalRotation);
	
	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
}

void CameraControl_RestoreToFrontView(CameraControl* This)
{
	Quat TotalRotation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	
	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
}

void CameraControl_RestoreToBackView(CameraControl* This)
{
	Quat TotalRotation = (Quat) {0.0f, 0.0f, 1.0f, 0.0f};
	
	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
	
}

void CameraControl_RestoreToTopView(CameraControl* This)
{
	Vec3f Axis = (Vec3f) {1.0f, 0.0f, 0.0f};
		
	Quat TotalRotation = (Quat) {0.0f, 0.0f, 0.0f, 0.0f};
	
	Quat_FromAxisAngle(&TotalRotation, &Axis, M_PI / 2.0f);

	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
}

void CameraControl_RestoreToBottomView(CameraControl* This)
{
	Vec3f Axis = (Vec3f) {1.0f, 0.0f, 0.0f};
		
	Quat TotalRotation = (Quat) {0.0f, 0.0f, 0.0f, 0.0f};	
	
	Quat_FromAxisAngle(&TotalRotation, &Axis, -M_PI / 2.0f);

	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
	
}

void CameraControl_RestoreToRightView(CameraControl* This)
{
	Vec3f Axis = (Vec3f) {0.0f, 1.0f, 0.0f};
		
	Quat TotalRotation = (Quat) {0.0f, 0.0f, 0.0f, 0.0f};
	
	Quat_FromAxisAngle(&TotalRotation, &Axis, -M_PI / 2.0f);

	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
}

void CameraControl_RestoreToLeftView(CameraControl* This)
{
	Vec3f Axis = (Vec3f) {0.0f, 1.0f, 0.0f};
	
	Quat TotalRotation = (Quat) {0.0f, 0.0f, 0.0f, 0.0f};
	
	Quat_FromAxisAngle(&TotalRotation, &Axis, M_PI / 2.0f);

	This->Animation = CAMERA_CONTROL_ANIMATION_ACTIVE;
	
	Vec3f Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	Vec3f Origin = (Vec3f) {0.0f, 0.0f, 0.0f};
	
	CameraControl_ComputeStuff(&Origin, 30.0f, &TotalRotation, NULL, &Position);
	
	This->MoveTo(This, &Position);
	This->ForwardTo(This, 30.0f);
	This->ShiftTo(This, &Origin);
	This->RotateTo(This, &TotalRotation);
}

void CameraControl_Update(CameraControl* This, float FrameTime)
{
	if (This->Animation == CAMERA_CONTROL_ANIMATION_ACTIVE)
	{
		if (This->IsMoving == TRUE)
		{
			This->MovingTime += FrameTime;
			
			if (This->MovingTime >= This->Settings->GetMovingDuration(This->Settings))
			{
				This->Position = This->MovingTo;
				This->IsMoving = FALSE;
			}
			else
			{
				Vec3f_Tween(&This->MovingFrom, &This->MovingTo, &This->Position, 
				This->Settings->GetMovingMode(This->Settings), This->MovingTime, 
				This->Settings->GetMovingDuration(This->Settings));
			}
		}

		if (This->IsShifting == TRUE)
		{
			This->ShiftingTime += FrameTime;
			
			if (This->ShiftingTime >= This->Settings->GetShiftingDuration(This->Settings))
			{
				This->Target = This->ShiftingTo;
				This->IsShifting = FALSE;
			}
			else
			{
				Vec3f_Tween(&This->ShiftingFrom, &This->ShiftingTo, &This->Target, 
				This->Settings->GetShiftingMode(This->Settings), This->ShiftingTime, 
				This->Settings->GetShiftingDuration(This->Settings));
			}	
		}
		
		if (This->IsForwarding == TRUE)
		{
			This->ForwardingTime += FrameTime;
			
			if (This->ForwardingTime >= This->Settings->GetShiftingDuration(This->Settings))
			{
				This->Distance = This->ForwardingTo;
				This->IsForwarding = FALSE;
			}
			else
			{
				This->Distance = Tween(This->Settings->GetForwardingMode(This->Settings), This->ForwardingTime,
									This->ForwardingFrom, This->ForwardingTo - This->ForwardingFrom, 
									This->Settings->GetForwardingDuration(This->Settings));
			}
		}
		
		if (This->IsTurning == TRUE)
		{
			This->TurningTime += FrameTime;
			
			if (This->TurningTime >= This->Settings->GetTurningDuration(This->Settings))
			{
				This->Orientation = This->TurningTo;
				This->IsTurning = FALSE;
			}
			else
			{
				float Proportion = Tween(This->Settings->GetTurningMode(This->Settings),
									This->TurningTime, 0.0f, 1.0f, 
									This->Settings->GetTurningDuration(This->Settings));
				
				Quat_Slerp(&This->TurningFrom, &This->TurningTo, Proportion, &This->Orientation);
				
			}
			
		}
		
		if (This->IsMoving == FALSE && This->IsShifting == FALSE && This->IsForwarding == FALSE && This->IsTurning == FALSE)
		{
			This->Animation = CAMERA_CONTROL_ANIMATION_NONE;
		}
	}
}

void CameraControl_StartDragging(CameraControl* This, float x, float y)
{
	This->Dragging = TRUE;	
	
	if (This->Mode == CAMERA_CONTROL_MODE_ROTATING)
	{
		This->StartMouseX = x;
		This->StartMouseY = y;
		This->PrevOrientation = This->Orientation;
	}
	else if (This->Mode == CAMERA_CONTROL_MODE_PANNING)
	{
		This->StartMouseX = 0.0f;
		This->StartMouseY = 0.0f;
	}
}

void CameraControl_TrackDragging(CameraControl* This, float x, float y)
{
	if (This->Dragging == TRUE)
	{
		if (This->Mode == CAMERA_CONTROL_MODE_ROTATING)
		{
			Vec3f V1, V2;			
			CameraControl_ComputeVector(This, This->StartMouseX, This->StartMouseY, &V1);
			CameraControl_ComputeVector(This, This->StartMouseX + x, This->StartMouseY + y, &V2);
			
			Quat Delta;
			Quat_FromVectors(&Delta, &V1, &V2);
			
			This->Orientation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
			Quat_Multiply(&This->Orientation, &Delta);
			Quat_Multiply(&This->Orientation, &This->PrevOrientation);
			Quat_Normalize(&This->Orientation);
		}
		else if (This->Mode == CAMERA_CONTROL_MODE_PANNING)
		{
			float dx = 0.0f;
			float dy = 0.0f;
			
			if (This->StartMouseX == 0.0f && This->StartMouseY == 0.0f)
			{
				This->StartMouseX = x;
				This->StartMouseY = y;
			}
			else
			{
				dx = (x - This->StartMouseX) * 0.05f;
				dy = (y - This->StartMouseY) * 0.05f;
				This->StartMouseX = x;
				This->StartMouseY = y;
			}
			
			//Vec3f Forward = (Vec3f) {-This->ViewMatrix.e31, -This->ViewMatrix.e32, -This->ViewMatrix.e33};
			Vec3f Left = (Vec3f) {-This->ViewMatrix.e11, -This->ViewMatrix.e12, -This->ViewMatrix.e13};
			Vec3f Up = (Vec3f) {-This->ViewMatrix.e21, -This->ViewMatrix.e22, -This->ViewMatrix.e23};
			
			Vec3f DeltaMove = Left;
			Vec3f_ProductByScalar(&DeltaMove, dx);
			Vec3f_ProductByScalar(&Up, -dy);
			Vec3f_Add(&DeltaMove, &Up);
			
			Vec3f_Add(&This->Position, &DeltaMove);
			Vec3f_Add(&This->Target, &DeltaMove);
        }
	}
}

void CameraControl_StopDragging(CameraControl* This)
{
	This->Dragging = FALSE;
	This->Mode = CAMERA_CONTROL_MODE_INVALID;
}

void CameraControl_ComputeMatrices(CameraControl* This)
{
	CameraControl_ComputeStuff(&This->Target, This->Distance, &This->Orientation, &This->ViewMatrix, &This->Position);
	Mat44f_Inverse(&This->ViewMatrix, &This->InvViewMatrix);
}

void CameraControl_Clone(CameraControl* This, CameraControl* Source, int Width, int Height)
{
	This->Mode = Source->Mode;
	This->Animation = Source->Animation;
	This->Dragging = Source->Dragging;
	
	This->ViewMatrix = Source->ViewMatrix;
	This->InvViewMatrix = Source->InvViewMatrix;
	
	This->Position = Source->Position;
	This->Target = Source->Target;
	This->Orientation = Source->Orientation;
	
	This->StartMouseX = Source->StartMouseX;
	This->StartMouseY = Source->StartMouseY;
	
	This->Distance = Source->Distance;
	This->MinDistance = Source->MinDistance;
	
	This->Radius = Source->Radius;
	
	This->SetViewWidth(This, Width);
	This->SetViewHeight(This, Height);
	
	This->PrevOrientation = Source->PrevOrientation;
	
	This->MovingFrom = Source->MovingFrom;
	This->MovingTo = Source->MovingTo;
	This->MovingTime = Source->MovingTime;
	This->IsMoving = Source->IsMoving;
	
	
	
	This->ShiftingFrom = Source->ShiftingFrom;
	This->ShiftingTo = Source->ShiftingTo;
	This->ShiftingTime = Source->ShiftingTime;
	This->IsShifting = Source->IsShifting;
	
	
	This->ForwardingFrom = Source->ForwardingFrom;
	This->ForwardingTo = Source->ForwardingTo;
	This->ForwardingTime = Source->ForwardingTime;
	This->IsForwarding = Source->IsForwarding;
	
	This->TurningFrom = Source->TurningFrom;
	This->TurningTo = Source->TurningTo;
	This->TurningTime = Source->TurningTime;
	This->IsTurning = Source->IsTurning;
	
}

void CameraControl_Init(CameraControl* This, CameraControlSettings* Settings)
{
	This->GetMode = CameraControl_GetMode;
	This->GetViewMatrix = CameraControl_GetViewMatrix;
	This->GetInvViewMatrix = CameraControl_GetInvViewMatrix;
	
	This->SetMode = CameraControl_SetMode;
	
	This->SetRadius = CameraControl_SetRadius;
	This->SetViewWidth = CameraControl_SetViewWidth;
	This->SetViewHeight = CameraControl_SetViewHeight;
	
	This->MoveTo = CameraControl_MoveTo;
	This->ShiftTo = CameraControl_ShiftTo;
	This->RotateTo = CameraControl_RotateTo;
	This->ForwardTo = CameraControl_ForwardTo;
	
    This->RestoreToPerspectiveView = CameraControl_RestoreToPerspectiveView;
    This->RestoreToFrontView = CameraControl_RestoreToFrontView;
    
    This->RestoreToBackView = CameraControl_RestoreToBackView;
    This->RestoreToTopView = CameraControl_RestoreToTopView;
    This->RestoreToBottomView = CameraControl_RestoreToBottomView;
    
    This->RestoreToRightView = CameraControl_RestoreToRightView;
    This->RestoreToLeftView = CameraControl_RestoreToLeftView;
    
    This->Update = CameraControl_Update;
    
    This->StartDragging = CameraControl_StartDragging;
    This->StopDragging = CameraControl_StopDragging;
    
    This->TrackDragging = CameraControl_TrackDragging;
    
	This->ComputeMatrices = CameraControl_ComputeMatrices;
	This->Clone = CameraControl_Clone;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	This->Settings = Settings;
	This->SetMode(This, CAMERA_CONTROL_MODE_INVALID);
	This->Animation = CAMERA_CONTROL_ANIMATION_NONE;
	
	This->Radius = 100.0f;
	This->ViewWidth = 0;
	This->ViewHeight = 0;
	This->HalfViewWidth = 0.0f;
	This->HalfViewHeight = 0.0f;
	This->PrevOrientation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	
	This->StartMouseX = 0.0f;
	This->StartMouseY = 0.0f;
	
	This->Distance = 0.1f;
	This->MinDistance = 0.1f;

	This->Dragging = FALSE;
	Mat44f_Identity(&This->ViewMatrix);
	Mat44f_Identity(&This->InvViewMatrix);
	This->Position = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->Target = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->Orientation = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for position movement
	
	This->MovingFrom = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->MovingTo = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->MovingTime = 0.0f;
	This->IsMoving = FALSE;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for target movement (shift)
	
	This->ShiftingFrom = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->ShiftingTo = (Vec3f) {0.0f, 0.0f, 0.0f};
	This->ShiftingTime = 0.0f;
	This->IsShifting = FALSE;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for forwarding using distance between position and target
	
	This->ForwardingFrom = 0.0f;
	This->ForwardingTo = 0.0f;
	This->ForwardingTime = 0.0f;
	This->IsForwarding = FALSE;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for rotation
	
	This->TurningFrom = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	This->TurningTo = (Quat) {1.0f, 0.0f, 0.0f, 0.0f};
	This->TurningTime = 0.0f;
	This->IsTurning = FALSE;
}

