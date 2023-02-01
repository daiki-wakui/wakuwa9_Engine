#pragma once
#include "CollisonPrimitive.h"

class Collison
{
public:

	//���ƕ���
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	//�_�ƎO�p�`
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle,
		Vector3* closest);

	//���ƎO�p�`
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		Vector3* inter = nullptr);
};

