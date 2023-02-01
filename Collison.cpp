#include "Collison.h"
#include <cmath>

bool Collison::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
    Vector3 distV = sphere.center;
    float dist = distV.dot(plane.normal) - plane.distance;

    if (fabsf(dist) > sphere.radius) return false;

    if (inter) {
        *inter = -dist * plane.normal;
        *inter += sphere.center;
    }

    return true;
}
