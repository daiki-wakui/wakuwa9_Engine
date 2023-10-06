#pragma once
class Easing
{
public:

	static float EaseOut(float timer, float maxtimer);
	static float EaseIn(float timer, float maxtimer);
	static float EaseOutCubic(float timer, float maxtimer);
	static float EaseInCubic(float timer, float maxtimer);
	static float EaseOutBack(float timer, float maxtimer);
};

