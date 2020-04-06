#ifndef RAY_H
#define RAY_H

#include "../../pch.h"
#include "BoundingBox.h"
#include "CollisionDetection.h"

struct Ray
{
	DirectX::XMFLOAT3 st;
	DirectX::XMFLOAT3 dir;
	float t;

	inline Ray() 
	{
		st = DirectX::XMFLOAT3();
		dir = DirectX::XMFLOAT3();
		t = 0.0f;
	}

	inline Ray(DirectX::XMFLOAT3 st_, DirectX::XMFLOAT3 dir_)
	{
		st = st_;
		dir = dir_;
		t = 0.0f;
	}

	inline Ray& operator=(const Ray& r_)
	{
		st = r_.st;
		dir = r_.dir;
		t = 0.0f;
	}

	inline bool isColliding(BoundingBox* box_)
	{
		t = -1.0f;

		return CollisionDetection::RayOBBIntersection(this, box_);
	}
};

#endif
