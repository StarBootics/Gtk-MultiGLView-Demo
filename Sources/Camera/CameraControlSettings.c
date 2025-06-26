/*
 * CameraControlSettings.c
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

#include "CameraControlSettings.h"

TrackBallMode CameraControlSettings_GetTrackBallMode(CameraControlSettings* This)
{
	return This->Mode;
}

float CameraControlSettings_GetMovingDuration(CameraControlSettings* This)
{
	return This->MovingDuration;
}

TweenEffect CameraControlSettings_GetMovingMode(CameraControlSettings* This)
{
	return This->MovingMode;
}

float CameraControlSettings_GetShiftingDuration(CameraControlSettings* This)
{
	return This->ShiftingDuration;
}

TweenEffect CameraControlSettings_GetShiftingMode(CameraControlSettings* This)
{
	return This->ShiftingMode;	
}

float CameraControlSettings_GetForwardingDuration(CameraControlSettings* This)
{
	return This->ForwardingDuration;
}

TweenEffect CameraControlSettings_GetForwardingMode(CameraControlSettings* This)
{
	return This->ForwardingMode;
}

float CameraControlSettings_GetTurningDuration(CameraControlSettings* This)
{
	return This->TurningDuration;
}

TweenEffect CameraControlSettings_GetTurningMode(CameraControlSettings* This)
{
	return This->TurningMode;
}

void CameraControlSettings_Init(CameraControlSettings* This)
{
	This->GetTrackBallMode = CameraControlSettings_GetTrackBallMode;
	
	This->GetMovingDuration = CameraControlSettings_GetMovingDuration;
	This->GetMovingMode = CameraControlSettings_GetMovingMode;
	
	This->GetShiftingDuration = CameraControlSettings_GetShiftingDuration;
	This->GetShiftingMode = CameraControlSettings_GetShiftingMode;
	
	This->GetForwardingDuration = CameraControlSettings_GetForwardingDuration;
	This->GetForwardingMode = CameraControlSettings_GetForwardingMode;
	
	This->GetTurningDuration = CameraControlSettings_GetTurningDuration;
	This->GetTurningMode = CameraControlSettings_GetTurningMode;
	
	This->Mode = TRACK_BALL_MODE_ARC;
	This->MovingDuration = 0.75f;
	This->MovingMode = TE_QUADRATIC_EASE_IN_OUT;
	This->ShiftingDuration = 0.75f;
	This->ShiftingMode = TE_QUADRATIC_EASE_IN_OUT;
	This->ForwardingDuration = 0.75f;
	This->ForwardingMode = TE_QUADRATIC_EASE_IN_OUT;
	This->TurningDuration = 0.75f;
	This->TurningMode = TE_QUADRATIC_EASE_IN_OUT;
}

