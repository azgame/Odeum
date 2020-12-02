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
		float maxDistance = -FLT_MAX;

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

struct Collider2D
{

public:
	Collider2D(std::vector<Vector2>& vertices) { c_vertices = s_vertices = vertices; };

	// This returns the furthest point in a vector of vertices using a given direction
	inline Vector2 FindFurthestPoint(Vector2 direction)
	{
		Vector2 maxPoint;
		float maxDistance = -FLT_MAX;

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

	inline void Transpose(Vector2 t) {
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = s_vertices[i] + t;
		}
	}

	inline void Rotate(float angle)
	{
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = Vector2(c_vertices[i].GetX() * cos(angle) - c_vertices[i].GetY() * sin(angle), c_vertices[i].GetX() * sin(angle) + c_vertices[i].GetY() * cos(angle));
		}
	}

private:

	std::vector<Vector2> c_vertices;
	std::vector<Vector2> s_vertices;
};

// GJK Math Functions - https://www.youtube.com/watch?v=MDusDn8oTSE&ab_channel=CaseyMuratori for resource
// lots of helper functions
namespace Math {

	inline Vector2 Support(Collider2D* cA, Collider2D* cB, Vector2 direction)
	{
		return cA->FindFurthestPoint(direction) - cB->FindFurthestPoint(-direction);
	}

	inline Vector3 Support(Collider* cA, Collider* cB, Vector3 direction)
	{
		return cA->FindFurthestPoint(direction) - cB->FindFurthestPoint(-direction);
	}

	inline bool SameDirection(const Vector2& direction, const Vector2& ao)
	{
		return Math::Dot(direction, ao) > 0;
	}

	inline bool SameDirection(const Vector3& direction, const Vector3& ao)
	{
		return Math::Dot(direction, ao) > 0;
	}

	inline bool Line(Simplex<Vector2>& points, Vector2& direction)
	{
		Vector2 a = Vector2(points[0]);
		Vector2 b = Vector2(points[1]);

		Vector2 ab = b - a;
		Vector2 ao = -a;

		if (SameDirection(ab, ao))
		{
			direction = Math::TripleProduct(Math::TripleProduct(ab, ao, ao), ab, ab);
		}
		else
		{
			points = { a };
			direction = ao;
		}

		return false;
	}

	inline bool Line(Simplex<Vector3>& points, Vector3& direction)
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

	inline bool Triangle(Simplex<Vector2>& points, Vector2& direction)
	{
		Vector2 a = points[0];
		Vector2 b = points[1];
		Vector2 c = points[2];

		Vector2 ab = b - a;
		Vector2 ac = c - a;
		Vector2 ao = -a;

		Vector2 abc = Math::TripleProduct(ab, ac, ac);

		if (SameDirection(Math::TripleProduct(abc, ac, ac), ao))
		{
			if (SameDirection(ac, ao))
			{
				points = { a, c };
				direction = Math::TripleProduct(Math::TripleProduct(ac, ao, ao), ac, ac);
			}
			else
			{
				return Line(points = { a, b }, direction);
			}
		}
		else
		{
			if (SameDirection(Math::TripleProduct(ab, abc, abc), ao))
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

	inline bool Triangle(Simplex<Vector3>& points, Vector3& direction) {
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

	inline bool Tetrahedron(Simplex<Vector3>& points, Vector3& direction)
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

	inline bool NextSimplex(Simplex<Vector2>& points, Vector2& direction)
	{
		switch (points.Size())
		{
		case 2: return Line(points, direction);
		case 3: return Triangle(points, direction);
		default: return false;
		}
	}

	inline bool NextSimplex(Simplex<Vector3>& points, Vector3& direction)
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