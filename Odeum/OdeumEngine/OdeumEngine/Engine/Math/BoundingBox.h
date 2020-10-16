#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "D3DMath.h"


struct OrientedBoundingBox
{
	Matrix3 basis; // represents the orientation of the three axis. An AABB would be [xaxis (1, 0, 0), yaxis (0, 1, 0), zaxis (0, 0, 1)]. Rotate these to get the orientation of the obb
	Vector3 center; // represents the position of the obb
	Vector3 halfExtents; // center to bounds. to find faces, use plane p = center +/- (basis + extent) where the resulting vector is the plane normal and d is the extent
	// if confused, read these comments with this image: https://www2.cs.duke.edu/courses/cps124/spring04/notes/12_collisions/collision_detection.pdf page 13

	OrientedBoundingBox()
		: basis(kIdentity), center(Vector3(kIdentity) * 0.5f), halfExtents(Vector3(kIdentity) * 0.5f) {}
	

};

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
		Vector3 minCorner = TransformPoint(min, transform);
		Vector3 maxCorner = TransformPoint(max, transform);
		Vector3 bMinCorner = TransformPoint(bbox.min, bbox.transform);
		Vector3 bMaxCorner = TransformPoint(bbox.max, bbox.transform);

		return (minCorner.GetX() <= bMaxCorner.GetX() && maxCorner.GetX() >= bMinCorner.GetX()) &&
			(minCorner.GetY() <= bMaxCorner.GetY() && maxCorner.GetY() >= bMinCorner.GetY()) &&
			(minCorner.GetZ() <= bMaxCorner.GetZ() && maxCorner.GetZ() >= bMinCorner.GetZ());
	}

	Vector3 TransformPoint(Vector3 Point, Matrix4 Transform)
	{
		Vector3 v(transform.GetX().GetW(), transform.GetY().GetW(), transform.GetZ().GetW());
		return v + Point;
	}

private:
};

#endif