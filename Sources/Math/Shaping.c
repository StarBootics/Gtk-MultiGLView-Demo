/*
 * Shapings.c
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

#include <math.h>
#include "Shaping.h"

static float Shaping_Linear(float t)
{
	return t;
}

static float Shaping_Squared(float t)
{
	return t * t;
}

static float Shaping_Cubed(float t)
{
	return t * t * t;
}

static float Shaping_SquareRoot(float t)
{
	return sqrtf(t);
}

static float Shaping_CubicRoot(float t)
{
	return cbrtf(t);
}

static float Shaping_SmoothStep(float t)
{
	t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;  // Clamp t to [0,1]
	 
	float One_Minus_t = 1.0f - t;
	float VA = t * t;
	float VB = 1.0f - One_Minus_t * One_Minus_t;
	
	return ((VA) + (t) * ((VB) - (VA)));
}

// SmootherStep function: 6t^5 - 15t^4 + 10t^3
static float Shaping_SmootherStep(float t) 
{
    t = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;  // Clamp t to [0,1]
    
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

static float Shaping_QuadraticEaseOut(float t)
{
	float One_Minus_t = 1.0f - t;
	
	return 1.0f - One_Minus_t * One_Minus_t;
}

static float Shaping_Parabola(float t)
{
	return powf(4.0f * t * (1.0f - t), 2.0f);
}

static float Shaping_Triangle(float t)
{
	return 1.0f - 2.0f * fabsf(t - 0.5f);
}

static float Shaping_ElasticOut(float t)
{
	return sinf(-13.0f * (t + 1.0f) * (M_PI/2.0f)) * powf(2.0f, -10.0f * t) + 1.0f;
}

static float Shaping_BounceOut(float t)
{
	float Result = 0.0f;
	float nl = 7.5625f;
	float dl = 2.75f;
	
	if (t < 1.0f / dl)
	{
		Result = nl * t * t;
	}
	else if (t < 2.0f / dl)
	{
		t -= 1.5f / dl;
		Result = nl * t * t + 0.75f;
	}
	else if (t < 2.5f / dl)
	{
		t -= 2.25f / dl;
		Result = nl * t * t + 0.9375f;
	}
	else
	{
		t -= 2.625f / dl;
		Result = nl * t * t + 0.984375f;
	}
	
	return Result;
}

typedef float (*ShapingFunction)(float);

static ShapingFunction ShapingJumpTable[SE_COUNT] = {
	Shaping_Linear,
	Shaping_Squared,
	Shaping_Cubed,
	Shaping_SquareRoot,
	Shaping_CubicRoot,
	Shaping_SmoothStep,
	Shaping_SmootherStep,
	Shaping_QuadraticEaseOut,
	Shaping_Parabola,
	Shaping_Triangle,
	Shaping_ElasticOut,
	Shaping_BounceOut
};

float Shaping(ShapingEffect Effect, float t) 
{
    if (Effect < 0 || Effect >= SE_COUNT) 
    {
        return 0.0f;  // Return 0.0f value for invalid effect
    }
    
    return ShapingJumpTable[Effect](t);
}

