#pragma once
#include "CollisonPrimitive.h"

class Collison
{
public:

	//‹…‚Æ•½–Ê
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

};

