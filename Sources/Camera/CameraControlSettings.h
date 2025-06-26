/*
 * CameraControlSettings.h
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

#ifndef CAMERA_CONTROL_SETTINGS_H
#define CAMERA_CONTROL_SETTINGS_H

#include "Tween.h"

typedef enum
{
	TRACK_BALL_MODE_ARC,
	TRACK_BALL_MODE_PROJECT
} TrackBallMode; 

typedef struct CameraControlSettings CameraControlSettings;

struct CameraControlSettings
{
	TrackBallMode Mode;
	
	float MovingDuration;
	TweenEffect MovingMode;
	float ShiftingDuration;
	TweenEffect ShiftingMode;
	float ForwardingDuration;
	TweenEffect ForwardingMode;
	float TurningDuration;
	TweenEffect TurningMode;
	
	TrackBallMode (*GetTrackBallMode)(CameraControlSettings*);
	float (*GetMovingDuration)(CameraControlSettings*);
	TweenEffect (*GetMovingMode)(CameraControlSettings*);
	float (*GetShiftingDuration)(CameraControlSettings*);
	TweenEffect (*GetShiftingMode)(CameraControlSettings*);
	float (*GetForwardingDuration)(CameraControlSettings*);
	TweenEffect (*GetForwardingMode)(CameraControlSettings*);
	float (*GetTurningDuration)(CameraControlSettings*);
	TweenEffect (*GetTurningMode)(CameraControlSettings*);
	
	
	
};

void CameraControlSettings_Init(CameraControlSettings*);

#endif

