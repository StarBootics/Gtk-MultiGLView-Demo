/*
 * Tween.h
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

#ifndef TWEEN_H
#define TWEEN_H

typedef enum
{
	TE_LINEAR,
	
	TE_EXPONENTIAL_EASE_OUT,
	TE_EXPONENTIAL_EASE_IN,
	TE_EXPONENTIAL_EASE_IN_OUT,
	TE_EXPONENTIAL_EASE_OUT_IN,

	TE_CIRCULAR_EASE_OUT,
	TE_CIRCULAR_EASE_IN,
	TE_CIRCULAR_EASE_IN_OUT,
	TE_CIRCULAR_EASE_OUT_IN,

	TE_QUADRATIC_EASE_OUT,
	TE_QUADRATIC_EASE_IN,
	TE_QUADRATIC_EASE_IN_OUT,
	TE_QUADRATIC_EASE_OUT_IN,

	TE_SINUSOIDAL_EASE_OUT,
	TE_SINUSOIDAL_EASE_IN,
	TE_SINUSOIDAL_EASE_IN_OUT,
	TE_SINUSOIDAL_EASE_OUT_IN,

	TE_CUBIC_EASE_OUT,
	TE_CUBIC_EASE_IN,
	TE_CUBIC_EASE_IN_OUT,
	TE_CUBIC_EASE_OUT_IN,

	TE_QUARTIC_EASE_OUT,
	TE_QUARTIC_EASE_IN,
	TE_QUARTIC_EASE_IN_OUT,
	TE_QUARTIC_EASE_OUT_IN,

	TE_QUINTIC_EASE_OUT,
	TE_QUINTIC_EASE_IN,
	TE_QUINTIC_EASE_IN_OUT,
	TE_QUINTIC_EASE_OUT_IN,

	TE_ELASTIC_EASE_OUT,
	TE_ELASTIC_EASE_IN,
	TE_ELASTIC_EASE_IN_OUT,
	TE_ELASTIC_EASE_OUT_IN,

	TE_BOUNCE_EASE_OUT,
	TE_BOUNCE_EASE_IN,
	TE_BOUNCE_EASE_IN_OUT,
	TE_BOUNCE_EASE_OUT_IN,

	TE_BACK_EASE_OUT,
	TE_BACK_EASE_IN,
	TE_BACK_EASE_IN_OUT,
	TE_BACK_EASE_OUT_IN,
	
	TE_COUNT
} TweenEffect;

float Tween(TweenEffect Effect, float CurrentTime, float Start, float Delta, float Duration);

#endif
