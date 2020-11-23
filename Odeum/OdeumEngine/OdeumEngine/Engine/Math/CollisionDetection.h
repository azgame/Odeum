#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <memory>
#include "../Core/Camera.h"

struct Ray;
struct BoundingBox;
struct OrientedBoundingBox;
struct Simplex;
struct Collider;

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
	static bool RayOBBIntersection(Ray& ray, OrientedBoundingBox& box, Vector4* IntersectionPlane);
	static void RayOBBIntersectionPlane(Ray& ray, OrientedBoundingBox& box, Vector4* IntersectionPlane);
	// should update this so it takes in any object with at least 4 vertices
	static bool GJKCollisionDetection(Collider* s1, Collider* s2);
};

#endif