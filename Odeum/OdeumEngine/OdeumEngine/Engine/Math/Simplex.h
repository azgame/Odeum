#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "D3DMath.h"
#include <array>

template <typename T>
struct Simplex
{
private:
	std::array<T, 4> s_points;
	//unsigned s_size;
	size_t s_size;

public:
	Simplex() : s_points({ T(), T(), T(), T() }), s_size(0) {}
	//inline Simplex(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4) { points[0] = p1; points[1] = p2; points[2] = p3; points[3] = p4; }

	Simplex& operator= (std::initializer_list<T> list)
	{
		for (auto l = list.begin(); l != list.end(); l++)
		{
			s_points[std::distance(list.begin(), l)] = *l;
		}
		s_size = list.size();

		return *this;
	}

	inline void Push_Front(T p)
	{
		s_points = { p, s_points[0], s_points[1], s_points[2] };
		s_size = std::min((int)s_size + 1, 4);
	}

	inline void ReplacePoint(int i, T p)
	{
		s_points[i] = p;
	}

	T& operator[] (int i) { return s_points[i]; }

	size_t Size() const { return s_size; }

	auto Begin() const { return s_points.begin(); }
	auto End() const { return s_points.end() - (4 - s_size); }	
};


#endif