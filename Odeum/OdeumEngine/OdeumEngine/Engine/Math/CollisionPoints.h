#ifndef COLLISIONPOINTS_H
#define COLLISIONPOINTS_H

#include "D3DMath.h"

struct CollisionPoints
{
	Vector3 normal;
	float penetrationDepth;
	bool hasCollision;
};

#endif
