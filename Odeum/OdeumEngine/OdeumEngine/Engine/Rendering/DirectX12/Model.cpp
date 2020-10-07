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
	m_pMaterials = new Material[m_details.materialCount];
	memset(m_pMaterials, 0, sizeof(Material) * m_details.materialCount);

	for (unsigned int mIndex = 0; mIndex < scene->mNumMaterials; mIndex++)
	{
		const aiMaterial* sourceMat = scene->mMaterials[mIndex];
		Material& destMat = m_pMaterials[mIndex];

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

		aiString texDiffusePath;
		aiString texSpecularPath;
		aiString texEmissivePath;
		aiString texNormalPath;
		aiString texLightmapPath;
		aiString texReflectionPath;
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texDiffusePath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), texSpecularPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), texEmissivePath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), texNormalPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), texLightmapPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0), texReflectionPath);

		char* pRem = nullptr;

		strncpy_s(destMat.diffuseTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.diffuseTextureFile, texDiffusePath.C_Str(), 127);
		pRem = strrchr(destMat.diffuseTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		strncpy_s(destMat.specularTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.specularTextureFile, texSpecularPath.C_Str(), 127);
		pRem = strrchr(destMat.specularTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		strncpy_s(destMat.emissiveTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.emissiveTextureFile, texEmissivePath.C_Str(), 127);
		pRem = strrchr(destMat.emissiveTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		strncpy_s(destMat.normalTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.normalTextureFile, texNormalPath.C_Str(), 127);
		pRem = strrchr(destMat.normalTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		strncpy_s(destMat.lightmapTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.lightmapTextureFile, texLightmapPath.C_Str(), 127);
		pRem = strrchr(destMat.lightmapTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		strncpy_s(destMat.reflectionTextureFile, "Engine/Resources/Textures/", 127);
		strncat_s(destMat.reflectionTextureFile, texReflectionPath.C_Str(), 127);
		pRem = strrchr(destMat.reflectionTextureFile, '.');
		while (pRem != nullptr && *pRem != 0) *(pRem++) = 0; // remove extension

		aiString matName;
		sourceMat->Get(AI_MATKEY_NAME, matName);
		strncpy_s(destMat.name, matName.C_Str(), 127);
	}

	m_details.meshCount = scene->mNumMeshes;
	m_pMesh = new Mesh[m_details.meshCount];
	memset(m_pMesh, 0, sizeof(Vertex) * m_details.meshCount);

	for (unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
	{
		const aiMesh* sourceMesh = scene->mMeshes[mIndex];
		Mesh& destMesh = m_pMesh[mIndex];

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
		destMesh.vertexOffset = m_details.vertexCount;
		destMesh.vertexCount = sourceMesh->mNumVertices;
				
		destMesh.indexDataByteOffset = m_details.indexDataByteSize;
		destMesh.indexOffset = m_details.indexCount;
		destMesh.indexCount = sourceMesh->mNumFaces * 3;

		m_details.vertexDataByteSize += destMesh.vertexStride * destMesh.vertexCount;
		m_details.vertexCount += destMesh.vertexCount;
		m_details.indexDataByteSize += sizeof(uint16_t) * destMesh.indexCount;
		m_details.indexCount += destMesh.indexCount;
	}

	m_pVertexData = new Vertex[m_details.vertexCount];
	m_pIndexData = new uint16_t[m_details.indexCount];

	for (unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
	{
		const aiMesh* sourceMesh = scene->mMeshes[mIndex];
		Mesh& destMesh = m_pMesh[mIndex];

		Vertex* vertex = m_pVertexData + destMesh.vertexOffset;

		for (unsigned int v = 0; v < sourceMesh->mNumVertices; v++)
		{
			if (sourceMesh->mVertices)
			{
				vertex->position = DirectX::XMFLOAT3(sourceMesh->mVertices[v].x, sourceMesh->mVertices[v].y, sourceMesh->mVertices[v].z);
			}
			else
			{
				ASSERT(false, "No position, wtf?");
			}

			if (sourceMesh->mTextureCoords[0])
			{
				vertex->uvcoords = DirectX::XMFLOAT2(sourceMesh->mTextureCoords[0][v].x, sourceMesh->mTextureCoords[0][v].y);
			}
			else
			{
				vertex->uvcoords = DirectX::XMFLOAT2(0.0f, 0.0f);
			}

			if (sourceMesh->mNormals)
			{
				vertex->normal = DirectX::XMFLOAT3(sourceMesh->mNormals[v].x, sourceMesh->mNormals[v].y, sourceMesh->mNormals[v].z);
			}
			else
			{
				ASSERT(false, "Assimp is supposed to generate normals, so if there are none its a problem");
			}

			if (sourceMesh->mTangents)
			{
				vertex->tangent = DirectX::XMFLOAT3(sourceMesh->mTangents[v].x, sourceMesh->mTangents[v].y, sourceMesh->mTangents[v].z);
			}
			else
			{
				vertex->tangent = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			}

			if (sourceMesh->mBitangents)
			{
				vertex->bitangent = DirectX::XMFLOAT3(sourceMesh->mBitangents[v].x, sourceMesh->mBitangents[v].y, sourceMesh->mBitangents[v].z);
			}
			else
			{
				vertex->bitangent = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			}

			vertex++;
		}

		uint16_t* indice = m_pIndexData + destMesh.indexOffset;

		for (unsigned int fIndex = 0; fIndex < sourceMesh->mNumFaces; fIndex++)
		{
			ASSERT(sourceMesh->mFaces[fIndex].mNumIndices == 3, "Must be triangle indicies");

			*indice++ = sourceMesh->mFaces[fIndex].mIndices[0];
			*indice++ = sourceMesh->mFaces[fIndex].mIndices[1];
			*indice++ = sourceMesh->mFaces[fIndex].mIndices[2];
		}
	}

	m_vertexBuffer.Create("Vertex buffer", m_details.vertexCount, m_vertexStride, m_pVertexData);
	m_indexBuffer.Create("Index buffer", m_details.indexCount, sizeof(uint16_t), m_pIndexData);

	// Texture Loading
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
	m_pMesh[0].indexCount = numIndices_;
	m_pMesh[0].indexDataByteOffset = 0;
	m_pMesh[0].vertexCount = numVertices_;
	m_pMesh[0].vertexDataByteOffset = 0;
}

Model::Mesh& Model::GetMesh(int index)
{
	return m_pMesh[index];
}
