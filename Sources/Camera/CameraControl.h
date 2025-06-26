/*
 * CameraControl.h
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

#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include "Vec3f.h"
#include "Mat44f.h"
#include "Quat.h"
#include "CameraControlSettings.h"

#ifndef FALSE
	#define FALSE 0
#endif
	
#ifndef TRUE
	#define TRUE 1
#endif

typedef enum
{
	CAMERA_CONTROL_MODE_INVALID,
	CAMERA_CONTROL_MODE_ROTATING,
	CAMERA_CONTROL_MODE_PANNING
} CameraControlMode;

typedef enum 
{
	CAMERA_CONTROL_ANIMATION_NONE,
	CAMERA_CONTROL_ANIMATION_ACTIVE
} CameraControlAnimation;

typedef struct CameraControl CameraControl;

struct CameraControl
{
	CameraControlSettings* Settings;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Camera informations
	
	CameraControlMode Mode;
	CameraControlAnimation Animation;
	
	int Dragging;

	Mat44f ViewMatrix;
	Mat44f InvViewMatrix;
	Vec3f Position;
	Vec3f Target;
	Quat Orientation;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Control informations
	
	float StartMouseX;
	float StartMouseY;
		
	float Distance; // Distance between the Camera position and Target
	float MinDistance; // Minimum Distance between the Camera position and Target
		
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Trackball informations
	
	float Radius;
	int ViewWidth, ViewHeight;
	float HalfViewWidth, HalfViewHeight;
	Quat PrevOrientation;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for position movement
	
	Vec3f MovingFrom;
	Vec3f MovingTo;
	float MovingTime;
	int IsMoving;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for target movement (shift)
	
	Vec3f ShiftingFrom;
	Vec3f ShiftingTo;
	float ShiftingTime;
	int IsShifting;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for forwarding using distance between position and target
	
	float ForwardingFrom;
	float ForwardingTo;
	float ForwardingTime;
	int IsForwarding;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// for rotation
	
	Quat TurningFrom;
	Quat TurningTo;
	float TurningTime;
	int IsTurning;
	
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Methods
	
	CameraControlMode (*GetMode)(CameraControl*);
	Mat44f* (*GetViewMatrix)(CameraControl*);
	Mat44f* (*GetInvViewMatrix)(CameraControl*);
	
	void (*SetMode)(CameraControl*, CameraControlMode);
	void (*SetRadius)(CameraControl*, float);
	void (*SetViewWidth)(CameraControl*, int);
	void (*SetViewHeight)(CameraControl*, int);
	
	
	void (*MoveTo)(CameraControl*, Vec3f*);
	void (*ShiftTo)(CameraControl*, Vec3f*);
	void (*RotateTo)(CameraControl*, Quat*);
	void (*ForwardTo)(CameraControl*, float);
    
    void (*RestoreToPerspectiveView)(CameraControl*);
    void (*RestoreToFrontView)(CameraControl*);    
    void (*RestoreToBackView)(CameraControl*);
    void (*RestoreToTopView)(CameraControl*);
    void (*RestoreToBottomView)(CameraControl*);
    void (*RestoreToRightView)(CameraControl*);
    void (*RestoreToLeftView)(CameraControl*);
    
    void (*Update)(CameraControl*, float);
    
    void (*StartDragging)(CameraControl*, float, float);
    void (*TrackDragging)(CameraControl*, float, float);
    void (*StopDragging)(CameraControl*);
    
	void (*ComputeMatrices)(CameraControl*);
	
	void (*Clone)(CameraControl*, CameraControl*, int, int);
};

void CameraControl_Init(CameraControl*, CameraControlSettings*);

#endif

