#pragma once
#include "CollisonPrimitive.h"

class Collison
{
public:

	//球と平面
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	//点と三角形
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle,
		Vector3* closest);

	//球と三角形
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		Vector3* inter = nullptr);
};

