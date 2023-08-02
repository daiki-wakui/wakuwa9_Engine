#include "Easing.h"
#include <math.h>

float Easing::EaseOut(float timer, float maxtimer)
{
	timer = (float)fmin(timer, maxtimer);
	float t = timer / maxtimer;

	return (float)sin((t * 3.14f) / 2.0f);
}

float Easing::EaseIn(float timer, float maxtimer)
{
	timer = (float)fmin(timer, maxtimer);
	float t = timer / maxtimer;

	return 1.0f - (float)cos((t * 3.14f) / 2);
}

float Easing::EaseOutCubic(float timer, float maxtimer)
{
	timer = (float)fmin(timer, maxtimer);
	float t = timer / maxtimer;

	return 1.0f - (float)pow(1.0f - t, 3);
}

float Easing::EaseInCubic(float timer, float maxtimer)
{
	timer = (float)fmin(timer, maxtimer);
	float t = timer / maxtimer;

	return t * t * t;
}

