#ifndef SHAPES_H
#define SHAPES_H

#include "D3DIncludes.h"
#include "Colour.h"

using namespace DirectX;

struct Vertex
{
	Vertex() {}

	XMFLOAT3 position;
	XMFLOAT2 uvcoords;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
};

enum ShapeTypes
{
	NoShape = 0,
	CubeShape,
	SphereShape,

	NumShapes
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
};

#endif