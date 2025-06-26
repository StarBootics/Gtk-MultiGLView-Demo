/*
 * Tween.c
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
#include "Tween.h"

static float TweenLinear(float CurrentTime, float Start, float Delta, float Duration)
{
	return Delta * CurrentTime / Duration + Start;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenExponentialEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	if (CurrentTime == Duration)
	{
		return Start + Delta;
	}
	
	return Delta * (-powf(2.0f, -10.0f * CurrentTime / Duration) + 1.0f) + Start;
}

static float TweenExponentialEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if (CurrentTime == 0.0f)
	{
		return Start;
	}
	
	return Delta * powf(2.0f, 10.0f * ( CurrentTime / Duration - 1.0f ) ) + Start;
}

static float TweenExponentialEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	if (CurrentTime == 0.0f)
	{
		return Start;
	}
	
	if (CurrentTime == Duration)
	{
		return Start + Delta;
	}
	
	CurrentTime /= (Duration / 2);
	
	if (CurrentTime < 1.0f)
	{
		return Delta / 2.0f * powf( 2.0f, 10.0f * (CurrentTime - 1.0f ) ) + Start;
	}
	
	CurrentTime -= 1.0f;
	
	return Delta / 2.0f * ( -powf( 2.0f, -10.0f * CurrentTime ) + 2.0f ) + Start;
}

static float TweenExponentialEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if (CurrentTime < (Duration / 2.0f))
	{
		return TweenExponentialEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenExponentialEaseIn((CurrentTime*2.0f) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenCircularEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration - 1.0f;
	
	return Delta * sqrtf(1.0f - CurrentTime * CurrentTime) + Start;
}

static float TweenCircularEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration;
	
	return -Delta * (sqrtf(1.0f - CurrentTime * CurrentTime ) - 1.0f) + Start;
}

static float TweenCircularEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	
	if (CurrentTime < 1.0f)
	{
		return -Delta / 2.0f * ( sqrtf( 1.0f - CurrentTime * CurrentTime ) - 1.0f ) + Start;
	}
	
	CurrentTime -= 2.0f;
	
	return Delta / 2.0f * ( sqrtf( 1.0f - CurrentTime * CurrentTime ) + 1.0f ) + Start;
}

static float TweenCircularEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f))
	{
		return TweenCircularEaseOut( CurrentTime * 2.0f, Start, Delta / 2.0f, Duration );
	}
	
	return TweenCircularEaseIn( ( CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration );
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenQuadraticEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
    return -Delta * CurrentTime * ( CurrentTime - 2.0f ) + Start;
}

static float TweenQuadraticEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
    return Delta * CurrentTime * CurrentTime + Start;
}

static float TweenQuadraticEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	
	if (CurrentTime < 1.0f)
	{
		return Delta / 2.0f * CurrentTime * CurrentTime + Start;
	}
	
	CurrentTime -= 1.0f;
	
	return -Delta / 2.0f * ( CurrentTime * ( CurrentTime - 2.0f ) - 1.0f) + Start;
}

static float TweenQuadraticEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
    if ( CurrentTime < (Duration / 2.0f) )
    {
      return TweenQuadraticEaseOut( CurrentTime * 2.0f, Start, Delta / 2.0f, Duration );
    }
    
    return TweenQuadraticEaseIn( ( CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration );
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenSinusoidalEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	return Delta * sinf( CurrentTime / Duration * ( M_PI / 2.0f ) ) + Start;
}

static float TweenSinusoidalEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	return -Delta * cosf(CurrentTime / Duration * ( M_PI / 2.0f ) ) + Delta + Start;
}

static float TweenSinusoidalEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f) )
	{
		return TweenSinusoidalEaseIn(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenSinusoidalEaseOut((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

static float TweenSinusoidalEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f) )
	{
		return TweenSinusoidalEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenSinusoidalEaseIn((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenCubicEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration - 1.0f;
	
	return Delta * ( CurrentTime * CurrentTime * CurrentTime + 1.0f ) + Start;
}

static float TweenCubicEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
    
    return Delta * CurrentTime * CurrentTime * CurrentTime + Start;
}

static float TweenCubicEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
    CurrentTime /= (Duration / 2.0f);
    
    if ( CurrentTime < 1.0f )
    {
      return Delta / 2.0f * CurrentTime * CurrentTime * CurrentTime + Start;
    }
    
    CurrentTime -= 2.0f;
    
    return Delta / 2.0f * (CurrentTime * CurrentTime * CurrentTime + 2.0f ) + Start;
}

static float TweenCubicEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f) )
	{
		return TweenCubicEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenCubicEaseIn((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenQuarticEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration - 1.0f;
	
	return -Delta * (CurrentTime * CurrentTime * CurrentTime * CurrentTime - 1.0f ) + Start;
}

static float TweenQuarticEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	return Delta * CurrentTime * CurrentTime * CurrentTime * CurrentTime + Start;
}

static float TweenQuarticEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	
    if ( CurrentTime < 1.0f )
    {
		return Delta / 2.0f * CurrentTime * CurrentTime * CurrentTime * CurrentTime + Start;
	}
	
	 CurrentTime -= 2.0f;
	 
	 return -Delta / 2.0f * (CurrentTime * CurrentTime * CurrentTime * CurrentTime - 2.0f ) + Start;
}

static float TweenQuarticEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if (CurrentTime < Duration / 2.0f )
	{
		return TweenQuarticEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenQuarticEaseIn((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenQuinticEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration - 1.0f;
	
	return Delta * (CurrentTime * CurrentTime * CurrentTime * CurrentTime * CurrentTime + 1.0f ) + Start;
}

static float TweenQuinticEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	return Delta * CurrentTime * CurrentTime * CurrentTime * CurrentTime * CurrentTime + Start;
}

static float TweenQuinticEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	
    if ( CurrentTime < 1.0f )
    {
		return Delta / 2.0f * CurrentTime * CurrentTime * CurrentTime * CurrentTime * CurrentTime + Start;
	}
	
	 CurrentTime -= 2.0f;
	 
	 return -Delta / 2.0f * (CurrentTime * CurrentTime * CurrentTime * CurrentTime * CurrentTime + 2.0f ) + Start;
}

static float TweenQuinticEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
    if ( CurrentTime < (Duration / 2.0f))
    {
      return TweenQuinticEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
    }
    
    return TweenQuinticEaseIn((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenElasticEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	if (CurrentTime == 1.0f)
	{
		return Start + Delta;
	}
	
	float p = Duration  * 0.3f;
	float s = p / 4.0f;
	
	return (Delta * powf( 2.0f, -10.0f * CurrentTime ) * sinf((CurrentTime * Duration - s) * ( 2.0f * M_PI ) / p ) + Delta + Start);
}

static float TweenElasticEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	if (CurrentTime == 1.0f)
	{
		return Start + Delta;
	}
	
	float p = Duration  * 0.3f;
	float s = p / 4.0f;
	
	return -(Delta * powf(2.0f, 10.0f * CurrentTime ) * sinf((CurrentTime * Duration - s ) * ( 2.0f * M_PI ) / p ) ) + Start;
}

static float TweenElasticEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	
	if (CurrentTime == 2.0f)
	{
		return Start + Delta;
	}
	
	float p = Duration * 0.45f;
	float s = p / 4;
	
	if (CurrentTime < 1.0f)
	{
		CurrentTime -= 1;
		return -0.5f * ( Delta * powf( 2.0f, 10.0f * CurrentTime ) * sinf( ( CurrentTime * Duration - s ) * ( 2.0f * M_PI ) / p ) ) + Start;
	}
	
	CurrentTime -= 1;
	
	return Delta * powf( 2.0f, -10.0f * CurrentTime ) * sinf( ( CurrentTime * Duration - s ) * ( 2.0f * M_PI ) / p ) * 0.5f + Delta + Start;
}


static float TweenElasticEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
    if ( CurrentTime < (Duration / 2.0f))
    {
      return TweenElasticEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
    }
    
    return TweenElasticEaseIn((CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenBounceEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	if (CurrentTime < (1.0f/2.75f))
	{
		return Delta * ( 7.5625f * CurrentTime * CurrentTime ) + Start;
	}
	else if (CurrentTime < ( 2.0f / 2.75f ) )
	{
		CurrentTime -= ( 1.5f / 2.75f ); 
		return Delta * ( 7.5625f * CurrentTime * CurrentTime + 0.75f ) + Start;
	}
	else if (CurrentTime < ( 2.5f / 2.75f ) )
	{
		CurrentTime -= ( 2.25f / 2.75f ); 
		return Delta * ( 7.5625f * CurrentTime * CurrentTime + 0.9375f ) + Start;
	}
	
	CurrentTime -= ( 2.25f / 2.75f );
	return Delta * ( 7.5625f * CurrentTime * CurrentTime + 0.984375f ) + Start;
}

static float TweenBounceEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	return Delta - TweenBounceEaseOut(Duration - CurrentTime, 0.0f, Delta, Duration ) + Start;
}

static float TweenBounceEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f))
	{
		return TweenBounceEaseIn(CurrentTime * 2.0f, 0.0f, Delta, Duration) * 0.5f + Start;
	}
	
	return TweenBounceEaseOut(CurrentTime * 2.0f - Duration, 0.0f, Delta, Duration) * 0.5f + Delta * 0.5f + Start;
}

static float TweenBounceEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f))
	{
		return TweenBounceEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenBounceEaseIn( (CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static float TweenBackEaseOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime = CurrentTime / Duration - 1.0f;
	
	return Delta * ( CurrentTime * CurrentTime * (2.70158f * CurrentTime + 1.70158f ) + 1.0f ) + Start;
}

static float TweenBackEaseIn(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= Duration;
	
	return Delta * CurrentTime * CurrentTime * ( 2.70158f * CurrentTime - 1.70158f ) + Start;
}

static float TweenBackEaseInOut(float CurrentTime, float Start, float Delta, float Duration)
{
	CurrentTime /= (Duration / 2.0f);
	float s = 2.5949095f;
	
	if (CurrentTime < 1.0f)
	{
		return Delta / 2.0f * ( CurrentTime * CurrentTime * ( ( s + 1.0f ) * CurrentTime - s ) ) + Start;
	}
	
	CurrentTime -= 2.0f;
	
	return Delta / 2.0f * ( CurrentTime * CurrentTime * ( ( s + 1.0f ) * CurrentTime + s ) + 2.0f ) + Start;
}

static float TweenBackEaseOutIn(float CurrentTime, float Start, float Delta, float Duration)
{
	if ( CurrentTime < (Duration / 2.0f))
	{
		return TweenBackEaseOut(CurrentTime * 2.0f, Start, Delta / 2.0f, Duration);
	}
	
	return TweenBackEaseIn( (CurrentTime * 2.0f ) - Duration, Start + Delta / 2.0f, Delta / 2.0f, Duration);
}

// Function pointer array (jump table)
typedef float (*TweenFunction)(float, float, float, float);

static TweenFunction TweenJumpTable[TE_COUNT] = {
	TweenLinear,
	
	TweenExponentialEaseOut,
	TweenExponentialEaseIn,
	TweenExponentialEaseInOut,
	TweenExponentialEaseOutIn,
	
	TweenCircularEaseOut,
	TweenCircularEaseIn,
	TweenCircularEaseInOut,
	TweenCircularEaseOutIn,
	
	TweenQuadraticEaseOut,
	TweenQuadraticEaseIn,
	TweenQuadraticEaseInOut,
	TweenQuadraticEaseOutIn,
	
	TweenSinusoidalEaseOut,
	TweenSinusoidalEaseIn,
	TweenSinusoidalEaseInOut,
	TweenSinusoidalEaseOutIn,
	
	TweenCubicEaseOut,
	TweenCubicEaseIn,
	TweenCubicEaseInOut,
	TweenCubicEaseOutIn,
	
	TweenQuarticEaseOut,
	TweenQuarticEaseIn,
	TweenQuarticEaseInOut,
	TweenQuarticEaseOutIn,
	
	TweenQuinticEaseOut,
	TweenQuinticEaseIn,
	TweenQuinticEaseInOut,
	TweenQuinticEaseOutIn,
	
	TweenElasticEaseOut,
	TweenElasticEaseIn,
	TweenElasticEaseInOut,
	TweenElasticEaseOutIn,
	
	TweenBounceEaseOut,
	TweenBounceEaseIn,
	TweenBounceEaseInOut,
	TweenBounceEaseOutIn,
	
	TweenBackEaseOut,
	TweenBackEaseIn,
	TweenBackEaseInOut,
	TweenBackEaseOutIn
};


// Main tween function
float Tween(TweenEffect Effect, float CurrentTime, float Start, float Delta, float Duration) 
{
    if (Effect < 0 || Effect >= TE_COUNT) 
    {
        return Start;  // Return start value for invalid effect
    }
    
    return TweenJumpTable[Effect](CurrentTime, Start, Delta, Duration);
}

