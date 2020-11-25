#ifndef RAY_H
#define RAY_H

#include "BoundingBox.h"
#include "CollisionDetection.h"

struct Ray
{
	Vector3 origin;
	Vector3 direction;
	float t;

	inline Ray() : origin(Vector3(kZero)), direction(Vector3(kZero)), t(0.0f)
	{
	}

	inline Ray(Vector3 Origin, Vector3 Direction) : origin(Origin), direction(Direction), t(0.0f)
	{
	}

	inline Ray& operator=(const Ray& ray)
	{
		origin = ray.origin;
		direction = ray.direction;
		t = 0.0f;
	}

	/*inline bool isColliding(BoundingBox& box)
	{
		t = -1.0f;

		return CollisionDetection::RayOBBIntersection(*this, box);
	}*/

	inline bool IsColliding(OrientedBoundingBox& box, Vector4* IntersectionPlane)
	{
		t = -1.0f;

		return CollisionDetection::RayOBBIntersection(*this, box, IntersectionPlane);
	}

	inline Vector3 FindPosition()
	{
		return origin + (direction * t);
	}
};

#endif