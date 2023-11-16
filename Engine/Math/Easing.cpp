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

float Easing::EaseOutBack(float timer, float maxtimer)
{
	float c1 = 1.70158f;
	float c3 = c1 + 1;

	timer = (float)fmin(timer, maxtimer);
	float t = timer / maxtimer;

	return 1 + c3 * (float)pow(t - 1, 3) + c1 * (float)pow(t - 1, 2);
}

float Easing::EaseOutQuint(float timer, float maxtimer)
{
	float t = timer / maxtimer;

	return 1 - (float)pow(1 - t, 5);
}

