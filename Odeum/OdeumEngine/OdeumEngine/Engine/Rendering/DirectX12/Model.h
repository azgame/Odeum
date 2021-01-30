#ifndef MODEL_H
#define MODEL_H

#include "D3DIncludes.h"
#include "Buffers/D3DBuffer.h"
#include "Shapes.h"
#include "../../Math/D3DMath.h"
#include "TextureManager.h"

class RenderComponent;

class Model
{
public:

	Model() :
		m_pMesh(nullptr),
		m_pMaterials(nullptr),
		m_pVertexData(nullptr),
		m_pIndexData(nullptr),
		m_srvs(nullptr),
		m_vertexStride(sizeof(Vertex)),
		m_flatColour(1.0f, 1.0f, 1.0f, 1.0f) {}

	~Model()
	{
		SAFE_DELETE(m_pMesh);
		SAFE_DELETE(m_pMaterials);
		SAFE_DELETE(m_pVertexData);
		SAFE_DELETE(m_pIndexData);
		SAFE_DELETE(m_srvs);
	}

	void Load();
	void LoadTextures();

	static const unsigned short maxFilePath = 128;

	struct ModelInfo
	{
		std::string fileName;
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
		uint32_t indexOffset;
		uint32_t vertexCount;
		uint32_t vertexOffset;
		uint32_t vertexStride;

		uint32_t materialIndex;
	};

	struct Material
	{
		Material()
		{
			strncpy_s(name, "", 0);
			strncpy_s(diffuseTextureFile, "", 0);
			strncpy_s(specularTextureFile, "", 0);
			strncpy_s(emissiveTextureFile, "", 0);
			strncpy_s(normalTextureFile, "", 0);
			strncpy_s(lightmapTextureFile, "", 0);
			strncpy_s(reflectionTextureFile, "", 0);
		}

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
	Colour			m_flatColour;
	Vertex*			m_pVertexData; // temp vertex storage for upload
	uint16_t*		m_pIndexData; // temp index storage for upload
	Material*		m_pMaterials;

	uint32_t		m_vertexStride;

	bool			isLoaded = false;

	D3D12_CPU_DESCRIPTOR_HANDLE* GetSRVs(uint32_t mIndex) const
	{
		return m_srvs + mIndex * 6;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE* m_srvs; // materials/textures
	StructuredBuffer m_vertexBuffer;
	ByteAddressBuffer m_indexBuffer;

	Matrix4 m_modelMatrix;

	void SetParent(RenderComponent* parent) { m_renderComponent = parent; }
	RenderComponent* m_renderComponent;
};

#endif