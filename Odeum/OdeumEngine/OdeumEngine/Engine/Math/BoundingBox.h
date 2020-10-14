#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "D3DMath.h"

struct BoundingBox
{
	Vector3 min, max;
	Matrix4 transform;

	BoundingBox()
	{
		min = Vector3(kZero);
		max = Vector3(kIdentity);
		transform = Matrix4(kIdentity);
	}

	BoundingBox(Vector3 Min, Vector3 Max, Matrix4 Transform)
	{
		min = Min;
		max = Max;
		transform = Transform;
	}

	bool Intersects(BoundingBox& bbox)
	{
		Vector3 minCorner = GetTransformedPoint(min, transform);
		Vector3 maxCorner = GetTransformedPoint(max, transform);
		Vector3 bMinCorner = GetTransformedPoint(bbox.min, bbox.transform);
		Vector3 bMaxCorner = GetTransformedPoint(bbox.max, bbox.transform);

		return (minCorner.GetX() <= bMaxCorner.GetX() && maxCorner.GetX() >= bMinCorner.GetX()) &&
			(minCorner.GetY() <= bMaxCorner.GetY() && maxCorner.GetY() >= bMinCorner.GetY()) &&
			(minCorner.GetZ() <= bMaxCorner.GetZ() && maxCorner.GetZ() >= bMinCorner.GetZ());
	}

private:
	Vector3 GetTransformedPoint(Vector3 Point, Matrix4 Transform)
	{
		Vector3 v(transform.GetW());
		return v + Point;
	}
};

#endif