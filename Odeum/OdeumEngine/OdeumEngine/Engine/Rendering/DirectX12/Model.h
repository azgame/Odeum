#ifndef MODEL_H
#define MODEL_H

#include "D3DIncludes.h"
#include "Buffers/D3DBuffer.h"
#include "Shapes.h"
#include "../../Math/D3DMath.h"
#include "TextureManager.h"

// Revert data to pointer arrays. Reasoning is that it allows you to send model as a single chunk down to the gpu 
// rather than the fragmented meshes that make up an entire model, and does so in a single gpu call

class Model
{
public:

	Model() :
		m_pMesh(nullptr),
		m_pMaterials(nullptr),
		m_pVertexData(nullptr),
		m_pIndexData(nullptr),
		m_srvs(nullptr),
		m_vertexStride(sizeof(Vertex)) {}

	~Model()
	{
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pMaterials);
		SAFE_DELETE(m_pVertexData);
		SAFE_DELETE(m_pIndexData);
		SAFE_DELETE(m_srvs);
	}

	void Load(std::string fileName);
	void Load(Vertex* pvData_, uint32_t numVertices_, uint32_t vStride_, uint16_t* piData_, uint32_t numIndices_);
	void LoadTextures();

	static const unsigned short maxFilePath = 128;

	static const uint8_t attrib_Position = 0;
	static const uint8_t attrib_TexCoord = 1;
	static const uint8_t attrib_Normal = 2;
	static const uint8_t attrib_Tangent = 3;
	static const uint8_t attrib_Bitangent = 4;

	struct VAttributes
	{
		uint16_t offset;
		uint16_t normalized;
		uint16_t components;
		uint16_t format;
	};

	struct ModelInfo
	{
		uint32_t meshCount;
		uint32_t materialCount;
		uint32_t vertexDataByteSize;
		uint32_t indexDataByteSize;
		uint32_t vertexCount;
		uint32_t indexCount;
	};

	struct Mesh
	{
		uint32_t indexCount;
		uint32_t indexDataByteOffset;
		uint32_t indexOffset;
		uint32_t vertexCount;
		uint32_t vertexDataByteOffset;
		uint32_t vertexOffset;
		uint32_t vertexStride;

		uint32_t materialIndex;

		VAttributes attributes[16];
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

		char name[128];
		char diffuseTextureFile[128];
		char specularTextureFile[128];
		char emissiveTextureFile[128];
		char normalTextureFile[128];
		char lightmapTextureFile[128];
		char reflectionTextureFile[128];
	};

	Mesh& GetMesh(int index);

	ModelInfo		m_details;
	Mesh*			m_pMesh;
	Vertex*			m_pVertexData; // temp vertex storage for upload
	uint16_t*		m_pIndexData; // temp index storage for upload
	Material*		m_pMaterials;

	uint32_t		m_vertexStride;

	D3D12_CPU_DESCRIPTOR_HANDLE* GetSRVs(uint32_t mIndex) const
	{
		return m_srvs + mIndex * 6;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE* m_srvs; // materials/textures
	StructuredBuffer m_vertexBuffer;
	ByteAddressedBuffer m_indexBuffer;

	Matrix4 m_modelMatrix;
};

#endif