#ifndef COLLIDER_H
#define COLLIDER_H

#include "D3DMath.h"
#include "../Math/CollisionPoints.h"

struct Collider
{
private:
	std::vector<Vector3> c_vertices;
	std::vector<Vector3> s_vertices;
	std::vector<Vector3> r_vertices;

public:
	Collider(std::vector<Vector3>& vertices) { c_vertices = s_vertices = r_vertices = vertices; };
	
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
			c_vertices[i] = r_vertices[i] + t;
		}
	}

	// this should be changed to update based on the rigibody's orientation
	/*inline void Rotate(Vector3 axis, float angle)
	{
		Matrix3 rotationMat = Matrix3(XMMatrixRotationAxis(axis, angle));
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = rotationMat * c_vertices[i];
		}
	}*/
	inline void Rotate(Quaternion q)
	{
		Matrix3 rotationMat = Matrix3(q);
		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = rotationMat * c_vertices[i];
			r_vertices[i] = rotationMat * s_vertices[i];
		}
	}

	inline void Rotate(Matrix4 m)
	{

		for (int i = 0; i < c_vertices.size(); i++)
		{
			c_vertices[i] = Vector3(m * c_vertices[i]);
			r_vertices[i] = Vector3(m * s_vertices[i]);
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
	Collider2D(std::initializer_list<Vector2> init) { c_vertices = s_vertices = init; }

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
		case 2: return Line(points, direction); // might not need
		case 3: return Triangle(points, direction);
		case 4: return Tetrahedron(points, direction);
		}

		// if something goes wrong
		return false;
	}

	// EPA Response Helpers

	// HELPER FUNCTIONS FOR GJK RESPONSE
	//https://blog.winter.dev/2020/epa-algorithm/ as reference

	inline void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
	{
		auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));

		if (reverse != edges.end()) {
			edges.erase(reverse);
		}

		else {
			edges.emplace_back(faces[a], faces[b]);
		}
	}

	inline std::pair<std::vector<Vector4>, size_t> GetFaceNormals(const std::vector<Vector3>& polytope, const std::vector<size_t>& faces)
	{
		std::vector<Vector4> normals;
		size_t minTriangle = 0;
		float  minDistance = FLT_MAX;

		for (size_t i = 0; i < faces.size(); i += 3) {
			Vector3 a = polytope[faces[i]];
			Vector3 b = polytope[faces[i + 1]];
			Vector3 c = polytope[faces[i + 2]];

			Vector3 normal = Math::Cross(b - a, c - a).Normalize();
			float distance = Math::Dot(normal, a);

			if (distance < 0) {
				normal *= -1;
				distance *= -1;
			}

			normals.emplace_back(normal, distance);

			if (distance < minDistance) {
				minTriangle = i / 3;
				minDistance = distance;
			}
		}

		return { normals, minTriangle };
	}

	inline CollisionPoints EPA(Collider* c1, Collider* c2, Simplex<Vector3>& simplex)
	{
		std::vector<Vector3> polytope(simplex.Begin(), simplex.End());
		std::vector<size_t> faces = {
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		// auto [normals, minFace] = GetFaceNormals(polytope, faces);
		std::pair<std::vector<Vector4>, size_t> faceNormals = GetFaceNormals(polytope, faces);

		Vector3 minNormal;
		float minDistance = FLT_MAX;

		while (minDistance == FLT_MAX)
		{
			//minNormal = normals[minFace].xyz();
			minNormal = Vector3(faceNormals.first[faceNormals.second]);
			//minDistance = normals[minFace].w;
			//minDistance = faceNormals.second; //testing
			minDistance = faceNormals.first[faceNormals.second].GetW();

			Vector3 supPoint = Math::Support(c1, c2, minNormal);
			float sDistance = Math::Dot(minNormal, supPoint);

			if (abs(sDistance - minDistance) > 0.001f)
			{
				minDistance = FLT_MAX;

				std::vector<std::pair<size_t, size_t>> uniqueEdges;

				for (size_t i = 0; i < faceNormals.first.size(); i++) {
					if (Math::SameDirection(Vector3(faceNormals.first[i]), supPoint)) {
						size_t f = i * 3;

						AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
						AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
						AddIfUniqueEdge(uniqueEdges, faces, f + 2, f);

						faces[f + 2] = faces.back(); faces.pop_back();
						faces[f + 1] = faces.back(); faces.pop_back();
						faces[f] = faces.back(); faces.pop_back();

						faceNormals.first[i] = faceNormals.first.back(); faceNormals.first.pop_back();

						i--;
					}
				}

				std::vector<size_t> newFaces;
				/* for(auto [edgeIndex1, edgeIndex2] : uniqueEdges){
					newFaces.push_back(edgeIndex1);
					newFaces.push_back(edgeIndex2);
					newFaces.push_back(polytope.size());
				}*/
				for (std::pair<size_t, size_t>edgeIndex : uniqueEdges) {
					newFaces.push_back(edgeIndex.first);
					newFaces.push_back(edgeIndex.second);
					newFaces.push_back(polytope.size());
				}

				polytope.push_back(supPoint);

				//auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
				std::pair<std::vector<Vector4>, size_t> newFaceNormals = GetFaceNormals(polytope, newFaces);

				float oldMinDistance = FLT_MAX;
				for (size_t i = 0; i < faceNormals.first.size(); i++) {
					if (faceNormals.first[i].GetW() < oldMinDistance) {
						oldMinDistance = faceNormals.first[i].GetW();
						faceNormals.second = i;
					}
				}

				if (newFaceNormals.first[newFaceNormals.second].GetW() < oldMinDistance) {
					faceNormals.second = newFaceNormals.second + faceNormals.first.size();
				}

				faces.insert(faces.end(), newFaces.begin(), newFaces.end());
				faceNormals.first.insert(faceNormals.first.end(), newFaceNormals.first.begin(), newFaceNormals.first.end());
			}
		}

		CollisionPoints points;
		points.normal = minNormal;
		points.penetrationDepth = minDistance + 0.001f;
		points.hasCollision = true;

		return points;
	}
}

#endif