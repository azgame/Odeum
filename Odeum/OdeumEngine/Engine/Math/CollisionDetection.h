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

	static Ray ScreenPosToWorldRay(DirectX::XMFLOAT2 mouseCoords_, DirectX::XMFLOAT2 screenSize_, Camera* camera_);
	static bool RayOBBIntersection(Ray* ray_, BoundingBox* box_);
};

#endif