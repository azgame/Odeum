#include "CollisionDetection.h"

#include "Ray.h"
#include "BoundingBox.h"
#include "Simplex.h"
#include "Collider.h"
#include "../Core/OdeumEngine.h"

#include <algorithm>

CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::ScreenPosToWorldRay(Vector2 MouseCoords, Vector2 ScreenSize, Camera& camera)
{
	Vector4 raystart_NDC((MouseCoords.GetX() / ScreenSize.GetX() - 0.5f) * 2.0f,
		((ScreenSize.GetY() - MouseCoords.GetY() + 32.0f) / ScreenSize.GetY() - 0.5f) * 2.0f, // DirectX screen grows downward, need to flip y values
		-1.0f, 1.0f);

	Vector4 rayend_NDC((MouseCoords.GetX() / ScreenSize.GetX() - 0.5f) * 2.0f,
		((ScreenSize.GetY() - MouseCoords.GetY() + 32.0f) / ScreenSize.GetY() - 0.5f) * 2.0f, // DirectX screen grows downward, need to flip y values
		0.0f, 1.0f);

	Matrix4 inverse = Matrix4(DirectX::XMMatrixInverse(nullptr, camera.GetViewProjMatrix()));

	Vector4 raystart_World = inverse * raystart_NDC;
	raystart_World /= raystart_World.GetW();

	Vector4 rayend_World = inverse * rayend_NDC;
	rayend_World /= rayend_World.GetW();

	Vector4 raydir_World = (rayend_World - raystart_World).Normalize();

	return Ray(Vector3(raystart_World), Vector3(raydir_World));
}

bool CollisionDetection::RayOBBIntersection(Ray& ray, OrientedBoundingBox& box, Vector4* IntersectionPlane)
{
	float tMin = OdeumEngine::Get().GetCamera().GetNearClipPlane();
	float tMax = OdeumEngine::Get().GetCamera().GetFarClipPlane();

	Vector3 obbPos(box.center);
	Vector3 delta = obbPos - ray.origin;

	Vector3 min = obbPos - box.GetMin();
	Vector3 max = obbPos - box.GetMax();

	Vector3 xAxis(box.basis.GetX());

	float e = Math::Dot(xAxis, delta);
	float f = Math::Dot(ray.direction, xAxis);

	// FIND INTERSECTION PLANES
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

	e = Math::Dot(yAxis, delta);
	f = Math::Dot(ray.direction, yAxis);

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

	e = Math::Dot(zAxis, delta);
	f = Math::Dot(ray.direction, zAxis);

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

	// FINDING WHICH PLANE THE INTERSECTION WAS ON
	// First we find out the position of the ray based on t
	Vector3 pos = ray.FindPosition();
	
	// Then we check to see which center plane is closest to the vector
	std::vector<Vector4> planes = box.GetPlanes();
	float shortestDistance = VERY_LARGE_FLOAT;
	float newDistance = 0;

	for (auto p : planes)
	{
		// find the center of each plane
		Vector3 center = box.center + Vector3(p.GetVec() * p.GetW());
		newDistance = (pos - center).Mag();
		if (fabs(newDistance) < fabs(shortestDistance)) {
			shortestDistance = newDistance;
			IntersectionPlane = &p;
		}
	}

	// I THINK WE ARE GOOOD
	return true;
}

// Assuming intersection with ray and box, ray with house intersection point
void CollisionDetection::RayOBBIntersectionPlane(Ray& ray, OrientedBoundingBox& box, Vector4* IntersectionPlane)
{
	std::vector<Vector4> planes = box.GetPlanes();

	Vector3 intersectionPoint = ray.origin - box.center + (ray.direction * ray.t);
	double dot;

	for (int i = 0; i < 6; i++)
	{
		dot = fabs(-Math::Dot(Vector3(planes[i]), intersectionPoint) + planes[i].GetW());
		
		if (dot < 0.05 && dot >= 0.0)
		{
			*IntersectionPlane = planes[i];
		}
	}
}

bool CollisionDetection::GJKCollisionDetection(ComplexCollider* cc1, ComplexCollider* cc2, Simplex<Vector3>& points)
{
	// Simplex is stored as an arrary of points, maximum of 4
	// Get initial point with any direction
	Vector3 supPoint = Math::Support(cc1->GetCollider(), cc2->GetCollider(), Vector3(1.0f, 0.0f, 0.0f));
	
	// Reset Simplex
	points = Simplex<Vector3>();
	// Add new point to the new simplex
	points.Push_Front(supPoint);

	// The new direction will be pointing towards the origin
	Vector3 direction = -supPoint;

	while (true)
	{
		supPoint = Math::Support(cc1->GetCollider(), cc2->GetCollider(), direction);

		if (Math::Dot(supPoint, direction) <= 0.0f)
		{
			// no collision
			return false;
		}

		points.Push_Front(supPoint);

		if (Math::NextSimplex(points, direction))
		{
			return true;
		}
	}

	return false;
}

//bool CollisionDetection::GJKCollisionDetection2D(Collider2D* c1, Collider2D* c2)
//{
//	// Get initial point with any direction
//	Vector2 supPoint = Math::Support(c1, c2, Vector2(0.0f, 1.0f));
//
//	// Simplex is stored as an arrary of points, maximum of 4
//	Simplex<Vector2> points;
//	// Add new point to the new simplex
//	points.Push_Front(supPoint);
//
//	// The new direction will be pointing towards the origin
//	Vector2 direction = -supPoint;
//
//	while (true)
//	{
//		supPoint = Math::Support(c1, c2, direction);
//
//		if (Math::Dot(supPoint, direction) <= 0.0f)
//		{
//			// no collision
//			return false;
//		}
//
//		points.Push_Front(supPoint);
//
//		if (Math::NextSimplex(points, direction))
//		{
//			return true;
//		}
//	}
//
//	return false;
//}

bool CollisionDetection::GJKCollisionDetection2D(Collider2D* s1, Collider2D* s2)
{
	size_t index = 0; // index of current vertex of simplex
	Vector2 a, b, c, ao, ab, ac, abperp, acperp, simplex[3];

	Vector2 supPoint = Math::Support(s1, s2, Vector2(0.0f, 1.0f));
	Vector2 direction = -supPoint;

	int counter = 0;

	a = simplex[0] = supPoint;

	while (true) {
		counter++;

		a = simplex[++index] = Math::Support(s1, s2, direction);

		if (Math::Dot(a, direction) <= 0)
			return false; // no collision

		ao = -a; // from point A to Origin is just negative A

		// simplex has 2 points (a line segment, not a triangle yet)
		if (index < 2) {
			b = simplex[0];
			ab = b - a; // from point A to B
			direction = Math::TripleProduct(ab, ao, ab); // normal to AB towards Origin
			if ((direction.GetX() * direction.GetX() + direction.GetY() * direction.GetY()) == 0)
				direction = Vector2(ab.GetY(), -ab.GetX());
			continue; // skip to next iteration
		}

		b = simplex[1];
		c = simplex[0];
		ab = b - a; // from point A to B
		ac = c - a; // from point A to C

		acperp = Math::TripleProduct(ab, ac, ac);

		if (Math::Dot(acperp, ao) >= 0) {

			direction = acperp; // new direction is normal to AC towards Origin
		}
		else {

			abperp = Math::TripleProduct(ac, ab, ab);

			if (Math::Dot(abperp, ao) < 0)
				return true; // collision

			simplex[0] = simplex[1]; // swap first element (point C)

			direction = abperp; // new direction is normal to AB towards Origin
		}

		simplex[1] = simplex[2]; // swap element in the middle (point B)
		--index;
	}

	return false;
}

bool CollisionDetection::SphereSphereCollisionDetection(SphereCollider* sc1, SphereCollider* sc2)
{
	float distance = (sc1->GetPosition() - sc2->GetPosition()).Mag();

	float sumOfRadius = sc1->GetRadius() + sc2->GetRadius();

	return distance < sumOfRadius;
}

bool CollisionDetection::SphereOBBCollisionDetection(SphereCollider* sc, BoxCollider* bc)
{
	// Get the box's closest point to sphere center
	Vector3 closestPoint = Math::Max(bc->GetBoundingBox()->GetMin(), Math::Min(sc->GetPosition(), bc->GetBoundingBox()->GetMax()));
	
	float distance = (closestPoint - sc->GetPosition()).Mag();

	return distance < sc->GetRadius();
}

bool CollisionDetection::OBBOBBBCollisionDetection(BoxCollider* bc1, BoxCollider* bc2)
{
	return bc1->GetBoundingBox()->Intersects(*bc2->GetBoundingBox());
}
