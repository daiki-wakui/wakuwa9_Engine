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

	//���C�ƕ���
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	//���C�ƎO�p�`
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, Vector3* inter = nullptr);

	//���C�Ƌ�
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, Vector3* inter = nullptr);
};

