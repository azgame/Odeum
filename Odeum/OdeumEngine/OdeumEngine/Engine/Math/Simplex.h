#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "D3DMath.h"
#include <array>

struct Simplex
{
private:
	std::array<Vector3, 4> s_points;
	//unsigned s_size;
	int s_size;

public:
	Simplex() : s_points({ Vector3(), Vector3(), Vector3(), Vector3() }), s_size(0) {}
	//inline Simplex(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4) { points[0] = p1; points[1] = p2; points[2] = p3; points[3] = p4; }

	Simplex& operator= (std::initializer_list<Vector3> list)
	{
		for (auto l = list.begin(); l != list.end(); l++)
		{
			s_points[std::distance(list.begin(), l)] = *l;
		}
		s_size = list.size();

		return *this;
	}

	void Push_Front(Vector3 p)
	{
		s_points = { p, s_points[0], s_points[1], s_points[2] };
		s_size = std::min(s_size + 1, 4);
	}

	Vector3& operator[] (int i) { return s_points[i]; }

	int Size() const { return s_size; }

	auto Begin() const { return s_points.begin(); }
	auto End() const { return s_points.end() - (4 - s_size); }	
};


#endif