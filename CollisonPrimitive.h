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

class Triangle
{
public:
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 normal;

	void ComputeNormal();
};

struct Ray
{
	Vector3 start;
	Vector3 dir = { 1.0f,0.0f,0.0f };

	
};