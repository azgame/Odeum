#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <memory>
#include "../Core/Camera.h"
#include "../CoreComponents/SphereCollider.h"
#include "../CoreComponents/BoxCollider.h"
#include "../CoreComponents/ComplexCollider.h"

struct Ray;
struct OrientedBoundingBox;
struct Collider;
struct Collider2D;

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

	static bool GJKCollisionDetection(ComplexCollider* cc1, ComplexCollider* cc2, Simplex<Vector3>& points);
	static bool GJKCollisionDetection2D(Collider2D* s1, Collider2D* s2);
	static bool SphereSphereCollisionDetection(SphereCollider* sc1, SphereCollider* sc2);
	static bool SphereOBBCollisionDetection(SphereCollider* sc, BoxCollider* bc);
	static bool OBBOBBBCollisionDetection(BoxCollider* bc1, BoxCollider* bc2);

};

#endif