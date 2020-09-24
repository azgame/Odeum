#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Load(std::string fileName)
{
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
		aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 0xFFFFFFFF);
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 0xFFFE);

	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_RemoveComponent |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ValidateDataStructure |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_SortByPType |
		aiProcess_FindInvalidData |
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph);

	if (scene == nullptr)
	{
		Debug::Error("Could not load model!", __FILENAME__, __LINE__);
		return;
	}
		
	m_details.materialCount = scene->mNumMaterials;
	m_pMaterials = new Material[m_details.materialCount];
	memset(m_pMaterials, 0, sizeof(Material) * m_details.materialCount);

}

void Model::Load(Vertex* pvData_, uint32_t numVertices_, uint32_t vStride_, uint16_t* piData_, uint32_t numIndices_)
{
	m_details = ModelInfo();
	m_details.meshCount = 1;
	m_details.vertexDataByteSize = numVertices_ * m_vertexStride;
	m_details.indexDataByteSize = numIndices_ * sizeof(uint16_t);

	m_vertexStride = vStride_;
	m_pVertexData = pvData_;
	m_pIndexData = piData_;

	m_vertexBuffer.Create("Vertex buffer", numVertices_, m_vertexStride, m_pVertexData);
	m_indexBuffer.Create("Index buffer", numIndices_, sizeof(uint16_t), m_pIndexData);

	m_pMesh = new Mesh();
	Mesh& mesh = *m_pMesh;
	mesh.indexCount = numIndices_;
	mesh.indexDataByteOffset = 0;
	mesh.vertexCount = numVertices_;
	mesh.vertexDataByteOffset = 0;
}

Model::Mesh& Model::GetMesh(int index)
{
	return m_pMesh[index];
}
