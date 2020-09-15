#ifndef MODEL_H
#define MODEL_H

#include "../Rendering/DirectX12/DXIncludes.h"
#include "../Rendering/DirectX12/Buffers/D3DBuffer.h"
#include "../Math/Vector.h"

struct Vertex
{
	Vertex(DirectX::XMFLOAT3 pos_, DirectX::XMFLOAT3 normal_) { position = pos_; normal = normal_; }
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

class Model
{
public:

	Model() :
		m_mesh(nullptr),
		m_pVertexData(nullptr),
		m_pIndexData(nullptr),
		m_vertexStride(0) {}

	~Model()
	{
		m_vertexBuffer.Destroy();
		m_indexBuffer.Destroy();
		delete[] m_mesh;
		delete[] m_pVertexData;
		delete[] m_pIndexData;
	}

	void Load(Vertex* pvData_, uint32_t vSize_, uint32_t vStride_, uint16_t* piData_, uint32_t iSize_);

	struct Mesh
	{
		uint32_t indexCount;
		uint32_t indexDataByteOffset;
		uint32_t vertexDataByteOffset;
	};

	Mesh* m_mesh;
	Vertex* m_pVertexData;
	uint16_t* m_pIndexData;
	StructuredBuffer m_vertexBuffer;
	ByteAddressedBuffer m_indexBuffer;
	uint32_t m_vertexStride;


};

#endif