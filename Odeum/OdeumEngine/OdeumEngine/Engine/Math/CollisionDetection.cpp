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

bool CollisionDetection::RayOBBIntersection(Ray& ray, OrientedBoundingBox& box)
{
	float tMin = OdeumEngine::Get().GetCamera().GetNearClipPlane();
	float tMax = OdeumEngine::Get().GetCamera().GetFarClipPlane();

	Vector3 obbPos(box.center);
	Vector3 delta = obbPos - ray.origin;

	Vector3 xAxis(box.basis.GetX());

	float e = Math::Dot(delta, xAxis);
	float f = Math::Dot(xAxis, ray.direction);

	Vector3 min = box.center - (box.basis.GetX() * box.halfExtents.GetX()) - (box.basis.GetY() * box.halfExtents.GetY()) - (box.basis.GetZ() * box.halfExtents.GetZ());
	Vector3 max = box.center + (box.basis.GetX() * box.halfExtents.GetX()) + (box.basis.GetY() * box.halfExtents.GetY()) + (box.basis.GetZ() * box.halfExtents.GetZ());

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + min.GetX()) / f;
		float t2 = (e + max.GetX()) / f;

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
		if (-e + min.GetX() > 0.0f || -e + max.GetX() < 0.0f)
			return false;

	Vector3 yAxis(box.basis.GetY());

	e = Math::Dot(delta, yAxis);
	f = Math::Dot(yAxis, ray.direction);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + min.GetY()) / f;
		float t2 = (e + max.GetY()) / f;

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
		if (-e + min.GetY() > 0.0f || -e + max.GetY() < 0.0f)
			return false;

	Vector3 zAxis(box.basis.GetZ());

	e = Math::Dot(delta, zAxis);
	f = Math::Dot(zAxis, ray.direction);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + min.GetZ()) / f;
		float t2 = (e + max.GetZ()) / f;

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
		if (-e + min.GetZ() > 0.0f || -e + max.GetZ() < 0.0f)
			return false;

	ray.t = tMin;

	return true;
}

// Assuming intersection with ray and box, ray with house intersection point
Vector4* CollisionDetection::RayOBBIntersectionPlane(Ray& ray, OrientedBoundingBox& box)
{
	Vector4* planes = new Vector4[6];
	box.GetPlanes(planes);

	Vector3 intersectionPoint = ray.origin + (ray.direction * ray.t);

	for (int i = 0; i < 6; i++)
	{
		if (fabs(Math::Dot(Vector3(planes[i]), intersectionPoint) < 0.05f))
			return &planes[i];
	}

	return nullptr;
}
