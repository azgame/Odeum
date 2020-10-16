#include "CollisionDetection.h"

#include "Ray.h"
#include "BoundingBox.h"

#include "../Core/OdeumEngine.h"

CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::ScreenPosToWorldRay(Vector2 MouseCoords, Vector2 ScreenSize, Camera& camera)
{
	Vector4 raystart_NDC((MouseCoords.GetX() / ScreenSize.GetX() - 0.5f) * 2.0f,
		((ScreenSize.GetY() - MouseCoords.GetY()) / ScreenSize.GetY() - 0.5f) * 2.0f, // DirectX screen grows downward, need to flip y values
		-1.0f, 1.0f);

	Vector4 rayend_NDC((MouseCoords.GetX() / ScreenSize.GetX() - 0.5f) * 2.0f,
		((ScreenSize.GetY() - MouseCoords.GetY()) / ScreenSize.GetY() - 0.5f) * 2.0f, // DirectX screen grows downward, need to flip y values
		0.0f, 1.0f);

	Matrix4 inverse = Matrix4(DirectX::XMMatrixInverse(nullptr, camera.GetViewProjMatrix()));

	Vector4 raystart_World = inverse * raystart_NDC;
	raystart_World = raystart_World / raystart_World.GetW();

	Vector4 rayend_World = inverse * rayend_NDC;
	rayend_World = rayend_World / rayend_World.GetW();

	Vector4 raydir_World = (rayend_World - raystart_World).Normalize();

	return Ray(Vector3(raystart_World), Vector3(raydir_World));
}

bool CollisionDetection::RayOBBIntersection(Ray& ray, BoundingBox& box)
{
	float tMin = OdeumEngine::Get().GetCamera().GetNearClipPlane();
	float tMax = OdeumEngine::Get().GetCamera().GetFarClipPlane();

	Vector3 obbPos(box.transform.GetW());
	Vector3 delta = obbPos - ray.origin;

	Vector3 xAxis(box.transform.GetX());

	float e = delta.Dot(xAxis);
	float f = xAxis.Dot(ray.direction);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box.min.GetX()) / f;
		float t2 = (e + box.max.GetX()) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)		
			tMax = t2;
		if (t1 > tMin)		
			tMin = t1;

		if (tMax < tMin)	
			return false;
	}
	else
		if (-e + box.min.GetX() > 0.0f || -e + box.max.GetX() < 0.0f)
			return false;

	Vector3 yAxis(box.transform.GetY());

	e = delta.Dot(yAxis);
	f = yAxis.Dot(ray.direction);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box.min.GetY()) / f;
		float t2 = (e + box.max.GetY()) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
			tMax = t2;
		if (t1 > tMin)
			tMin = t1;

		if (tMax < tMin)
			return false;
	}
	else
		if (-e + box.min.GetY() > 0.0f || -e + box.max.GetY() < 0.0f)
			return false;

	Vector3 zAxis(box.transform.GetZ());

	e = delta.Dot(zAxis);
	f = zAxis.Dot(ray.direction);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box.min.GetZ()) / f;
		float t2 = (e + box.max.GetZ()) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
			tMax = t2;
		if (t1 > tMin)
			tMin = t1;

		if (tMax < tMin)
			return false;
	}
	else
		if (-e + box.min.GetZ() > 0.0f || -e + box.max.GetZ() < 0.0f)
			return false;

	ray.t = tMin;

	return true;
}

struct Plane
{
	Plane() : plane(Vector4(kZero)) {} 
	Plane(Vector3& v0, Vector3& v1, Vector3& v2)
	{
		// Cross of E1 and E2 gives the normal of the plane
		Vector3 e1 = v1 - v0;
		Vector3 e2 = v2 - v1;
		Vector3 n = e1.Cross(e2);
		n.Normalize();

		// Find d with the negative dot product between the normal and a given point
		float d = -(n.Dot(v0));

		if (d == -0) d = 0;

		// Plane is the normal and d
		plane = Vector4(n, d);
	}

	Vector4 plane;
};

Vector4* CollisionDetection::RayOBBIntersectionPlane(Ray& ray, BoundingBox& box)
{
	// Construct 6 planes
	

	// Find where ray and normal of plane is v. close to 0

	return nullptr;
}
