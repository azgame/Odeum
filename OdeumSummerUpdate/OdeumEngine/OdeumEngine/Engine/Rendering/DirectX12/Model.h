#ifndef MODEL_H
#define MODEL_H

#include "DXIncludes.h"
#include "Buffers/D3DBuffer.h"
#include "../../Math/DXMath.h"

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
		/*m_vertexBuffer.Destroy();
		m_indexBuffer.Destroy();*/
		delete m_mesh;
		m_pVertexData = nullptr;
		m_pIndexData = nullptr;
	}

	// static const uint8_t 

	void Load(Vertex* pvData_, uint32_t vSize_, uint32_t vStride_, uint16_t* piData_, uint32_t iSize_);

	static const unsigned short maxFilePath = 128;

	struct ModelInfo
	{
		uint32_t meshCount;
		uint32_t materialCount;
		uint32_t vertexDataByteSize;
		uint32_t indexDataByteSize;
	};

	struct Mesh
	{
		uint32_t indexCount;
		uint32_t indexDataByteOffset;
		uint32_t vertexCount;
		uint32_t vertexDataByteOffset;
	};

	struct Material
	{
		Vector3 diffuse;
		Vector3 specular;
		Vector3 ambient;
		Vector3 emissive;
		Vector3 transparent;
		float opacity;
		float shininess;
		float specularStrength;
	};

	ModelInfo* m_details;
	Mesh* m_mesh;
	Material* m_material;

	Vertex* m_pVertexData; // temp vertex storage for upload
	uint16_t* m_pIndexData; // temp index storage for upload

	uint32_t m_vertexStride;
	StructuredBuffer m_vertexBuffer;
	ByteAddressedBuffer m_indexBuffer;

	Matrix4 m_modelMatrix;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_srvs; // materials/textures
};

#endif