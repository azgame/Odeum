#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Load(Vertex* pvData_, uint32_t vByteSize_, uint32_t vStride_, uint16_t* piData_, uint32_t iByteSize_)
{
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
		aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 0xFFFFFFFF);
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 0xFFFE);






	m_mesh = new Mesh();

	//m_pVertexData = new Vertex[vByteSize_ / m_vertexStride];

	m_vertexStride = vStride_;
	m_pVertexData = pvData_;
	m_pIndexData = piData_;

	m_vertexBuffer.Create("Vertex buffer", vByteSize_ / m_vertexStride, m_vertexStride, m_pVertexData);
	m_indexBuffer.Create("Index buffer", iByteSize_ / sizeof(uint16_t), sizeof(uint16_t), m_pIndexData);

	m_mesh->indexCount = iByteSize_ / sizeof(uint16_t);
	m_mesh->indexDataByteOffset = 0;
	m_mesh->vertexCount = vByteSize_ / m_vertexStride;
	m_mesh->vertexDataByteOffset = 0;
}
