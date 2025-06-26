/*
 * Shaping.h
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

#ifndef SHAPING_H
#define SHAPING_H

typedef enum 
{
	SE_LINEAR,
	SE_SQUARED,
	SE_CUBED,
	SE_SQUARE_ROOT,
	SE_CUBIC_ROOT,
	SE_SMOOTH_STEP,
	SE_SMOOTHER_STEP,
	SE_QUADRATIC_EASE_OUT,
	SE_PARABOLA,
	SE_TRIANGLE,
	SE_ELASTIC_OUT,
	SE_BOUNCE_OUT,
	SE_COUNT
} ShapingEffect;

float Shaping(ShapingEffect, float);

#endif

