#ifndef PLANE_H
#define PLANE_h

#include "Vector.h"

class Plane
{
public:
	Vector3 min, max, normal;
	float d; // distance from origin along the normal
};

#endif