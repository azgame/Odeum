#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <memory>
#include "../Core/Camera.h"

struct Ray;
struct BoundingBox;

class CollisionDetection
{
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator=(const CollisionDetection&) = delete;
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();

	static Ray ScreenPosToWorldRay(Vector2 MouseCoords, Vector2 ScreenSize, Camera& camera);
	static bool RayOBBIntersection(Ray& ray, BoundingBox& box);
	static Vector4* RayOBBIntersectionPlane(Ray& ray, BoundingBox& box);
};

#endif