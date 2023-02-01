#pragma once
#include "Vector3.h"

struct Sphere {
	Vector3 center;
	float radius = 1.0f;
};

struct Plane {
	Vector3 normal = { 0.0f,1.0f,0.0f };
	float distance = 0.0f;
};
