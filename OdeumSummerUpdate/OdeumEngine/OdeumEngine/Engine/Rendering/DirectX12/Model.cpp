#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Load(Vertex* pvData_, uint32_t numVertices_, uint32_t vStride_, uint16_t* piData_, uint32_t numIndices_)
{
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
		aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 0xFFFFFFFF);
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 0xFFFE);





	m_mesh = new Mesh();

	m_details = new ModelInfo();
	m_details->meshCount = 1;
	m_details->vertexDataByteSize = numVertices_ * m_vertexStride;
	m_details->indexDataByteSize = numIndices_ * sizeof(uint16_t);

	m_vertexStride = vStride_;
	m_pVertexData = pvData_;
	m_pIndexData = piData_;

	m_vertexBuffer.Create("Vertex buffer", numVertices_, m_vertexStride, m_pVertexData);
	m_indexBuffer.Create("Index buffer", numIndices_, sizeof(uint16_t), m_pIndexData);

	m_mesh->indexCount = numIndices_;
	m_mesh->indexDataByteOffset = 0;
	m_mesh->vertexCount = numVertices_;
	m_mesh->vertexDataByteOffset = 0;
}

Model::Mesh& Model::GetMesh(int index)
{
	return m_mesh[index];
}
