#ifndef SHAPES_H
#define SHAPES_H

#include "../../../pch.h"
#include "DXIncludes.h"
#include "../../Math/DXMath.h"

using namespace DirectX;

struct Vertex
{
	Vertex(XMFLOAT3 pos_) { position = pos_; }
	XMFLOAT3 position;
};

class Shape
{
public:
	virtual Vertex& GetVertices() = 0;
	virtual uint16_t& GetIndices() = 0;
	virtual size_t NumVertices() = 0;
	virtual size_t NumIndices() = 0;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class Cube : public Shape
{
public:

	Vertex& GetVertices() override
	{
		return *vertices.data();
	}

	uint16_t& GetIndices() override
	{
		return *indices.data();
	}

	size_t NumVertices() override
	{
		return vertices.size();
	}

	size_t NumIndices() override
	{
		return indices.size();
	}

	Cube()
	{
		vertices = {
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f)},

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, -1.0f, 1.0f)},
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f)},

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f)},
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f)},
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f)},

			{ XMFLOAT3(1.0f, -1.0f, 1.0f)},
			{ XMFLOAT3(1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, 1.0f)},

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, -1.0f, -1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, -1.0f)},
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f)},

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f)},
			{ XMFLOAT3(1.0f, -1.0f, 1.0f)},
			{ XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f)},
		};

		indices = {
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};
	}


};

#endif