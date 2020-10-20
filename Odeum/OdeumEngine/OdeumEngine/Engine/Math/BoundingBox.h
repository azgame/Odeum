#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "D3DMath.h"


struct OrientedBoundingBox
{
	Vector3 center; // represents the position of the obb
	Matrix3 basis; // represents the orientation of the three axis. An AABB would be [xaxis (1, 0, 0), yaxis (0, 1, 0), zaxis (0, 0, 1)]. Rotate these to get the orientation of the obb
	Vector3 halfExtents; // center to bounds. to find faces, use plane p = center +/- (basis + extent) where the resulting vector is the plane normal and d is the extent
	// if confused, read these comments with the image on page 13: https://www2.cs.duke.edu/courses/cps124/spring04/notes/12_collisions/collision_detection.pdf

	OrientedBoundingBox()
		: center(Vector3(kIdentity) * 0.5f), basis(kIdentity), halfExtents(Vector3(kIdentity) * 0.5f) 
	{
		basis.SetZ(Vector3(0.0f, 0.0f, -1.0f));
	}
	
	OrientedBoundingBox(Vector3 Center, Matrix3 Basis, Vector3 Extents)
		: center(Center), basis(Basis), halfExtents(Extents) {}

	OrientedBoundingBox(Vector3 min, Vector3 max)
	{
		basis = Matrix3(kIdentity);
		basis.SetZ(Vector3(0.0f, 0.0f, -1.0f));

		center = Vector3(0.5f * (max - min) + min);
		halfExtents = center - min;
	}

	bool Intersects(OrientedBoundingBox& Box)
	{
		Vector3 RPos;
		RPos = center - Box.center;

		return !(IsSeperatingPlane(RPos, basis.GetX(), Box) ||
			IsSeperatingPlane(RPos, basis.GetY(), Box) ||
			IsSeperatingPlane(RPos, basis.GetZ(), Box) ||
			IsSeperatingPlane(RPos, Box.basis.GetX(), Box) ||
			IsSeperatingPlane(RPos, Box.basis.GetY(), Box) ||
			IsSeperatingPlane(RPos, Box.basis.GetZ(), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetX(), Box.basis.GetX()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetX(), Box.basis.GetY()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetX(), Box.basis.GetZ()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetY(), Box.basis.GetX()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetY(), Box.basis.GetY()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetY(), Box.basis.GetZ()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetZ(), Box.basis.GetX()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetZ(), Box.basis.GetY()), Box) ||
			IsSeperatingPlane(RPos, Math::Cross(basis.GetZ(), Box.basis.GetZ()), Box));
	}

	// Fill an 6-sized array of vector4s with the planes of the obb
	void GetPlanes(Vector4* Planes)
	{
		Vector4* plane = Planes;
		*plane++ = Vector4(center + (basis.GetX() * -halfExtents.GetX()), -halfExtents.GetX());
		*plane++ = Vector4(center + (basis.GetX() * halfExtents.GetX()), halfExtents.GetX());
		*plane++ = Vector4(center + (basis.GetY() * -halfExtents.GetY()), -halfExtents.GetY());
		*plane++ = Vector4(center + (basis.GetY() * halfExtents.GetY()), halfExtents.GetY());
		*plane++ = Vector4(center + (basis.GetZ() * -halfExtents.GetZ()), -halfExtents.GetZ());
		*plane++ = Vector4(center + (basis.GetZ() * halfExtents.GetZ()), halfExtents.GetZ());
	}

	Vector3 GetMin()
	{
		return center - (basis.GetX() * halfExtents.GetX()) - (basis.GetY() * halfExtents.GetY()) - (basis.GetZ() * halfExtents.GetZ());
		
	}

	Vector3 GetMax()
	{
		return center + (basis.GetX() * halfExtents.GetX()) + (basis.GetY() * halfExtents.GetY()) + (basis.GetZ() * halfExtents.GetZ());
	}

	// Update with either a quaternion or a rotation matrix. Need to update each basis vector (basis -.getx(), -.gety(), -.getz()) with the new orientation
	// void UpdateOrientation() {}

private:

	bool IsSeperatingPlane(const Vector3& RPos, const Vector3& Plane, const OrientedBoundingBox& Box)
	{
		return fabs(Math::Dot(RPos, Plane)) >
			(fabs(Math::Dot(basis.GetX() * halfExtents.GetX(), Plane)) +
			fabs(Math::Dot(basis.GetY() * halfExtents.GetY(), Plane)) +
			fabs(Math::Dot(basis.GetZ() * halfExtents.GetZ(), Plane)) +
			fabs(Math::Dot(Box.basis.GetX() * Box.halfExtents.GetX(), Plane)) +
			fabs(Math::Dot(Box.basis.GetY() * Box.halfExtents.GetY(), Plane)) +
			fabs(Math::Dot(Box.basis.GetZ() * Box.halfExtents.GetZ(), Plane)));
	}
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