#ifndef COLLIDER_H
#define COLLIDER_H

#include "D3DMath.h"

struct Collider
{
private:
	std::vector<Vector3> c_vertices;
	std::vector<Vector3> s_vertices;

public:
	Collider(std::vector<Vector3>& vertices) { c_vertices = s_vertices = vertices; };
	
	// This returns the furthest point in a vector of vertices using a given direction
	inline Vector3 FindFurthestPoint(Vector3 direction)
	{
		Vector3 maxPoint;
		float maxDistance = -VERY_LARGE_FLOAT;

		for (auto v : c_vertices)
		{
			float distance = Math::Dot(v, direction);

			if (distance > maxDistance)
			{
				maxDistance = distance;
				maxPoint = v;
			}
		}
		return maxPoint;
	}

	inline void Transpose(Vector3 t) {
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = s_vertices[i] + t;
		}
	}

	inline void Rotate(Vector3 axis, float angle)
	{
		Matrix3 rotationMat = Matrix3(XMMatrixRotationAxis(axis, angle));
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = rotationMat * c_vertices[i];
		}
	}

	// testing
	inline void Print()
	{
		for (auto v : c_vertices)
		{
			v.Print();
			std::cout << std::endl;
		}
	}
};

// GJK Math Functions - https://www.youtube.com/watch?v=MDusDn8oTSE&ab_channel=CaseyMuratori for resource
// lots of helper functions
namespace Math {

	inline Vector3 Support(Collider* cA, Collider* cB, Vector3 direction)
	{
		return cA->FindFurthestPoint(direction) - cB->FindFurthestPoint(-direction);
	}

	inline bool SameDirection(const Vector3& direction, const Vector3& ao)
	{
		return Math::Dot(direction, ao) > 0;
	}

	inline bool Line(Simplex& points, Vector3& direction)
	{
		Vector3 a = points[0];
		Vector3 b = points[1];

		Vector3 ab = b - a;
		Vector3 ao = -a;

		if (SameDirection(ab, ao))
		{
			direction = Math::Cross(Math::Cross(ab, ao), ab);
		}
		else
		{
			points = { a };
			direction = ao;
		}
		
		return false;
	}

	inline bool Triangle(Simplex& points, Vector3& direction) {
		Vector3 a = points[0];
		Vector3 b = points[1];
		Vector3 c = points[2];

		Vector3 ab = b - a;
		Vector3 ac = c - a;
		Vector3 ao = -a;

		Vector3 abc = Math::Cross(ab, ac);

		if (SameDirection(Math::Cross(abc, ac), ao))
		{
			if (SameDirection(ac, ao))
			{
				points = { a, c };
				direction = Math::Cross(Math::Cross(ac, ao), ac);
			}
			else
			{
				return Line(points = { a, b }, direction);
			}
		}
		else
		{
			if (SameDirection(Math::Cross(ab, abc), ao))
			{
				return Line(points = { a, b }, direction);
			}
			else
			{
				if (SameDirection(abc, ao))
				{
					direction = abc;
				}
				else
				{
					points = { a, c, b };
					direction = -abc;
				}
			}
		}

		return false;
	}

	inline bool Tetrahedron(Simplex& points, Vector3& direction)
	{
		// points of tetrahedron we create
		Vector3 a = points[0];
		Vector3 b = points[1];
		Vector3 c = points[2];
		Vector3 d = points[3];

		// vectors between points
		Vector3 ab = b - a;
		Vector3 ac = c - a;
		Vector3 ad = d - a;
		Vector3 ao = -a;

		Vector3 abc = Math::Cross(ab, ac);
		Vector3 acd = Math::Cross(ac, ad);
		Vector3 adb = Math::Cross(ad, ab);

		if (SameDirection(abc, ao))
		{
			return Triangle(points = { a, b, c }, direction);
		}
		
		if (SameDirection(acd, ao))
		{
			return Triangle(points = { a, c, d }, direction);
		}

		if (SameDirection(adb, ao))
		{
			return Triangle(points = { a, d, b }, direction);
		}

		return true;
	}

	inline bool NextSimplex(Simplex& points, Vector3& direction)
	{
		switch (points.Size())
		{
		case 2: return Line(points, direction); // don't need this
		case 3: return Triangle(points, direction);
		case 4: return Tetrahedron(points, direction);
		}

		// if something goes wrong
		return false;
	}

}

#endif