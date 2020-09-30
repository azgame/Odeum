#ifndef MODEL_H
#define MODEL_H

#include "D3DIncludes.h"
#include "Buffers/D3DBuffer.h"
#include "Shapes.h"
#include "../../Math/D3DMath.h"

// Revert data to pointer arrays. Reasoning is that it allows you to send model as a single chunk down to the gpu 
// rather than the fragmented meshes that make up an entire model, and does so in a single gpu call

class Model
{
public:

	Model() :
		m_vertexStride(0) {}

	~Model()
	{
		m_meshes.clear();
		m_vertexData.clear();
		m_indexData.clear();
		m_materials.clear();
		m_srvs.clear();
	}

	void Load(std::string fileName);
	void Load(Vertex* pvData_, uint32_t numVertices_, uint32_t vStride_, uint16_t* piData_, uint32_t numIndices_);

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
	};

	struct Mesh
	{
		uint32_t indexCount;
		uint32_t indexDataByteOffset;
		uint32_t vertexCount;
		uint32_t vertexDataByteOffset;
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
	};

	Mesh& GetMesh(int index);

	ModelInfo m_details;
	std::vector<Mesh> m_meshes;
	std::vector<Vertex> m_vertexData; // temp vertex storage for upload
	std::vector<uint16_t> m_indexData; // temp index storage for upload

	std::vector<Material> m_materials;
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_srvs; // materials/textures

	uint32_t m_vertexStride;
	StructuredBuffer m_vertexBuffer;
	ByteAddressedBuffer m_indexBuffer;

	Matrix4 m_modelMatrix;
};

#endif