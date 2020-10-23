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
	{}
	
	OrientedBoundingBox(Vector3 Center, Matrix3 Basis, Vector3 Extents)
		: center(Center), basis(Basis), halfExtents(Extents) {}

	OrientedBoundingBox(Vector3 min, Vector3 max)
	{
		basis = Matrix3(kIdentity);

		halfExtents = Vector3(0.5f * (max - min));
		center = halfExtents + min;
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
	std::vector<Vector4> GetPlanes()
	{
		std::vector<Vector4> plane;
		plane.push_back(Vector4(basis.GetX(), halfExtents.GetX() * basis.GetX().GetX() + halfExtents.GetX() * basis.GetX().GetY() + halfExtents.GetX() * basis.GetX().GetZ()));
		plane.push_back(Vector4(-basis.GetX(), halfExtents.GetX() * basis.GetX().GetX() + halfExtents.GetX() * basis.GetX().GetY() + halfExtents.GetX() * basis.GetX().GetZ()));
		plane.push_back(Vector4(basis.GetY(), halfExtents.GetY() * basis.GetY().GetX() + halfExtents.GetY() * basis.GetY().GetY() + halfExtents.GetY() * basis.GetY().GetZ()));
		plane.push_back(Vector4(-basis.GetY(), halfExtents.GetY() * basis.GetY().GetX() + halfExtents.GetY() * basis.GetY().GetY() + halfExtents.GetY() * basis.GetY().GetZ()));
		plane.push_back(Vector4(basis.GetZ(), halfExtents.GetZ() * basis.GetZ().GetX() + halfExtents.GetZ() * basis.GetZ().GetY() + halfExtents.GetZ() * basis.GetZ().GetZ()));
		plane.push_back(Vector4(-basis.GetZ(), halfExtents.GetZ() * basis.GetZ().GetX() + halfExtents.GetZ() * basis.GetZ().GetY() + halfExtents.GetZ() * basis.GetZ().GetZ()));
		return plane;
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
	// Reference - https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
	void UpdateOrientation(Quaternion q) 
	{
		// find the angle and axis of the quaternion rotation and apply that rotation to the matrix
		// make sure the quaternion is normalized
		if (q.GetVector4().GetW() > 1.0f)
		{
			q = Quaternion(q.GetVector4().Normalize());
		}

		// these will be what we use to rotate the bounding box
		float angle, x, y, z;
		
		angle = 2.0f * acos(q.GetVector4().GetW());
		double s = sqrt(1.0f - q.GetVector4().GetW() * q.GetVector4().GetW());

		// avoid dividing by zero
		if (s < 0.0001f)
		{
			x = q.GetVector4().GetX();
			y = q.GetVector4().GetY();
			z = q.GetVector4().GetZ();
		}
		else 
		{
			// normalizes
			x = q.GetVector4().GetX() / (float)s;
			y = q.GetVector4().GetY() / (float)s;
			z = q.GetVector4().GetZ() / (float)s;
		}
		
		// rotate the basis vectors by angle on the (x, y, z) axis
		// create rotation matrices for each axis
		Matrix3 rot = Matrix3(DirectX::XMMatrixRotationAxis(Vector3(x, y, z), angle));

		// rotate each axis with the rotation matrix
		basis.SetX(rot * basis.GetX());
		basis.SetY(rot * basis.GetY());
		basis.SetZ(rot * basis.GetZ());
	}

	// ^^^ that might have been redundant, I REALLY HOPE THIS ISN'T ALL WE HAD TO DO 
	void UpdateOrientation2(Quaternion q)
	{
		// Matrix already has a constructor that builds a rotation matrix from a quaternion
		Matrix3 m = Matrix3(q);
		basis.SetX(m * basis.GetX());
		basis.SetY(m * basis.GetY());
		basis.SetZ(m * basis.GetZ());
	}

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