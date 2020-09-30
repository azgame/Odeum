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
		ERROR("Could not load model!")
		return;
	}
		
	m_details.materialCount = scene->mNumMaterials;
	m_materials.reserve(m_details.materialCount);

	for (unsigned int mIndex = 0; mIndex < scene->mNumMaterials; mIndex++)
	{
		const aiMaterial* sourceMat = scene->mMaterials[mIndex];
		Material& destMat = m_materials[mIndex];

		aiColor3D diffuse(1.0f, 1.0f, 1.0f);
		aiColor3D specular(1.0f, 1.0f, 1.0f);
		aiColor3D ambient(1.0f, 1.0f, 1.0f);
		aiColor3D emissive(1.0f, 1.0f, 1.0f);
		aiColor3D transparent(1.0f, 1.0f, 1.0f);
		float opacity = 1.0f;
		float shininess = 1.0f;
		float specularStrength = 1.0f;

		sourceMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		sourceMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		sourceMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		sourceMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		sourceMat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
		sourceMat->Get(AI_MATKEY_OPACITY, opacity);
		sourceMat->Get(AI_MATKEY_SHININESS, shininess);
		sourceMat->Get(AI_MATKEY_SHININESS_STRENGTH, specularStrength);

		destMat.diffuse = Vector3(diffuse.r, diffuse.g, diffuse.b);
		destMat.specular = Vector3(specular.r, specular.g, specular.b);
		destMat.ambient = Vector3(ambient.r, ambient.g, ambient.b);
		destMat.emissive = Vector3(emissive.r, emissive.g, emissive.b);
		destMat.transparent = Vector3(transparent.r, transparent.g, transparent.b);
		destMat.opacity = opacity;
		destMat.shininess = shininess;
		destMat.specularStrength = specularStrength;
	}

	m_details.meshCount = scene->mNumMeshes;
	m_meshes.reserve(m_details.meshCount);;

	for (unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
	{
		const aiMesh* sourceMesh = scene->mMeshes[mIndex];
		Mesh& destMesh = m_meshes[mIndex];

		ASSERT(sourceMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

		destMesh.materialIndex = sourceMesh->mMaterialIndex;

		destMesh.attributes[attrib_Position].offset = destMesh.vertexStride;
		destMesh.attributes[attrib_Position].normalized = 0;
		destMesh.attributes[attrib_Position].components = 3;
		destMesh.attributes[attrib_Position].format = 4;
		destMesh.vertexStride += sizeof(float) * 3;
				
		destMesh.attributes[attrib_TexCoord].offset = destMesh.vertexStride;
		destMesh.attributes[attrib_TexCoord].normalized = 0;
		destMesh.attributes[attrib_TexCoord].components = 2;
		destMesh.attributes[attrib_TexCoord].format = 4;
		destMesh.vertexStride += sizeof(float) * 2;
				
		destMesh.attributes[attrib_Normal].offset = destMesh.vertexStride;
		destMesh.attributes[attrib_Normal].normalized = 0;
		destMesh.attributes[attrib_Normal].components = 3;
		destMesh.attributes[attrib_Normal].format = 4;
		destMesh.vertexStride += sizeof(float) * 3;
				
		destMesh.attributes[attrib_Tangent].offset = destMesh.vertexStride;
		destMesh.attributes[attrib_Tangent].normalized = 0;
		destMesh.attributes[attrib_Tangent].components = 3;
		destMesh.attributes[attrib_Tangent].format = 4;
		destMesh.vertexStride += sizeof(float) * 3;
				
		destMesh.attributes[attrib_Bitangent].offset = destMesh.vertexStride;
		destMesh.attributes[attrib_Bitangent].normalized = 0;
		destMesh.attributes[attrib_Bitangent].components = 3;
		destMesh.attributes[attrib_Bitangent].format = 4;
		destMesh.vertexStride += sizeof(float) * 3;
				
		destMesh.vertexDataByteOffset = m_details.vertexDataByteSize;
		destMesh.vertexCount = sourceMesh->mNumVertices;
				
		destMesh.indexDataByteOffset = m_details.indexDataByteSize;
		destMesh.indexCount = sourceMesh->mNumFaces * 3;

		m_details.vertexDataByteSize += destMesh.vertexStride * destMesh.vertexCount;
		m_details.indexDataByteSize += sizeof(uint16_t) * destMesh.indexCount;
	}

	m_vertexData.reserve(m_details.vertexDataByteSize / sizeof(Vertex));
	m_indexData.reserve(m_details.indexDataByteSize / sizeof(uint16_t));

	for (unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
	{
		const aiMesh* sourceMesh = scene->mMeshes[mIndex];
		Mesh& destMesh = m_meshes[mIndex];

		for (unsigned int v = 0; v < sourceMesh->mNumVertices; v++)
		{
			Vertex& vertex = m_vertexData[v];
		}
	}


}

void Model::Load(Vertex* pvData_, uint32_t numVertices_, uint32_t vStride_, uint16_t* piData_, uint32_t numIndices_)
{
	m_details = ModelInfo();
	m_details.meshCount = 1;
	m_details.vertexDataByteSize = numVertices_ * m_vertexStride;
	m_details.indexDataByteSize = numIndices_ * sizeof(uint16_t);

	m_vertexStride = vStride_;
	m_vertexData = std::vector<Vertex>(pvData_, pvData_ + numVertices_);
	m_indexData = std::vector<uint16_t>(piData_, piData_ + numIndices_);

	m_vertexBuffer.Create("Vertex buffer", numVertices_, m_vertexStride, m_vertexData.data());
	m_indexBuffer.Create("Index buffer", numIndices_, sizeof(uint16_t), m_indexData.data());

	m_meshes.push_back(Mesh());
	m_meshes[0].indexCount = numIndices_;
	m_meshes[0].indexDataByteOffset = 0;
	m_meshes[0].vertexCount = numVertices_;
	m_meshes[0].vertexDataByteOffset = 0;
}

Model::Mesh& Model::GetMesh(int index)
{
	return m_meshes[index];
}
