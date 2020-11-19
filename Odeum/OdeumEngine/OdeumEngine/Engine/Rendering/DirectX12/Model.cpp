#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../Core/GameObject.h"

#include <thread>

void Model::Load()
{
	std::string fileName = m_details.fileName;

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

		aiString matName;
		aiString texDiffusePath;
		aiString texSpecularPath;
		aiString texEmissivePath;
		aiString texNormalPath;
		aiString texLightmapPath;
		aiString texReflectionPath;
		sourceMat->Get(AI_MATKEY_NAME, matName);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texDiffusePath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), texSpecularPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), texEmissivePath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT, 0), texNormalPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_LIGHTMAP, 0), texLightmapPath);
		sourceMat->Get(AI_MATKEY_TEXTURE(aiTextureType_REFLECTION, 0), texReflectionPath);

		strncpy_s(destMat.name, matName.C_Str(), 127);
		strncpy_s(destMat.diffuseTextureFile, texDiffusePath.C_Str(), 127);
		strncpy_s(destMat.specularTextureFile, texSpecularPath.C_Str(), 127);
		strncpy_s(destMat.emissiveTextureFile, texEmissivePath.C_Str(), 127);
		strncpy_s(destMat.normalTextureFile, texNormalPath.C_Str(), 127);
		strncpy_s(destMat.lightmapTextureFile, texLightmapPath.C_Str(), 127);
		strncpy_s(destMat.reflectionTextureFile, texReflectionPath.C_Str(), 127);
	}

	m_details.meshCount = scene->mNumMeshes;
	m_pMesh = new Mesh[m_details.meshCount];
	memset(m_pMesh, 0, sizeof(Mesh) * m_details.meshCount);

	for (unsigned int mIndex = 0; mIndex < scene->mNumMeshes; mIndex++)
	{
		const aiMesh* sourceMesh = scene->mMeshes[mIndex];
		Mesh& destMesh = m_pMesh[mIndex];

		ASSERT(sourceMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE);

		destMesh.materialIndex = sourceMesh->mMaterialIndex;
		destMesh.vertexStride = sizeof(float) * 14; // pos(3); uv(2); norm(3); tangent(3); bitangent(3);
				
		destMesh.vertexOffset = m_details.vertexCount;
		destMesh.vertexCount = sourceMesh->mNumVertices;
				
		destMesh.indexOffset = m_details.indexCount;
		destMesh.indexCount = sourceMesh->mNumFaces * 3;

		m_details.vertexDataByteSize += destMesh.vertexStride * destMesh.vertexCount;
		m_details.vertexCount += destMesh.vertexCount;
		m_details.indexDataByteSize += sizeof(uint16_t) * destMesh.indexCount;
		m_details.indexCount += destMesh.indexCount;
	}

	m_pVertexData = new Vertex[m_details.vertexCount];
	m_pIndexData = new uint16_t[m_details.indexCount];

	Vector3 vMin(kZero), vMax(kZero);

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
				if (vMin.GetX() > sourceMesh->mVertices[v].x) vMin.SetX(sourceMesh->mVertices[v].x);
				if (vMin.GetY() > sourceMesh->mVertices[v].y) vMin.SetY(sourceMesh->mVertices[v].y);
				if (vMin.GetZ() > sourceMesh->mVertices[v].z) vMin.SetZ(sourceMesh->mVertices[v].z);
				if (vMax.GetX() < sourceMesh->mVertices[v].x) vMax.SetX(sourceMesh->mVertices[v].x);
				if (vMax.GetY() < sourceMesh->mVertices[v].y) vMax.SetY(sourceMesh->mVertices[v].y);
				if (vMax.GetZ() < sourceMesh->mVertices[v].z) vMax.SetZ(sourceMesh->mVertices[v].z);
			}
			else
				ASSERT(false, "No position, wtf?");

			if (sourceMesh->mTextureCoords[0])
				vertex->uvcoords = DirectX::XMFLOAT2(sourceMesh->mTextureCoords[0][v].x, 1.0f - sourceMesh->mTextureCoords[0][v].y);
			else
				vertex->uvcoords = DirectX::XMFLOAT2(0.0f, 0.0f);

			if (sourceMesh->mNormals)
				vertex->normal = DirectX::XMFLOAT3(sourceMesh->mNormals[v].x, sourceMesh->mNormals[v].y, sourceMesh->mNormals[v].z);
			else
				ASSERT(false, "Assimp is supposed to generate normals, so if there are none its a problem");

			if (sourceMesh->mTangents)
				vertex->tangent = DirectX::XMFLOAT3(sourceMesh->mTangents[v].x, sourceMesh->mTangents[v].y, sourceMesh->mTangents[v].z);
			else
				vertex->tangent = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

			if (sourceMesh->mBitangents)
				vertex->bitangent = DirectX::XMFLOAT3(sourceMesh->mBitangents[v].x, sourceMesh->mBitangents[v].y, sourceMesh->mBitangents[v].z);
			else
				vertex->bitangent = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

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

	LoadTextures();

	m_gameObject->SetBoundingBox(vMin, vMax);
}

void Model::LoadTextures()
{
	m_srvs = new D3D12_CPU_DESCRIPTOR_HANDLE[m_details.materialCount * 6];
	const Texture* matTextures[6] = {};

	for (uint32_t mIndex = 0; mIndex < m_details.materialCount; mIndex++)
	{
		const Material& material = m_pMaterials[mIndex];

		std::thread t0;
		
		matTextures[0] = TextureManager::Get()->LoadFromFile(material.diffuseTextureFile);

		matTextures[1] = TextureManager::Get()->LoadFromFile(material.specularTextureFile);
		if (!matTextures[1]->isValid())
			matTextures[1] = matTextures[0];

		matTextures[3] = TextureManager::Get()->LoadFromFile(material.normalTextureFile);

		m_srvs[mIndex * 6 + 0] = matTextures[0]->GetSRV();
		m_srvs[mIndex * 6 + 1] = matTextures[1]->GetSRV();
		m_srvs[mIndex * 6 + 2] = matTextures[0]->GetSRV();
		m_srvs[mIndex * 6 + 3] = matTextures[3]->GetSRV();
		/*m_srvs[mIndex * 6 + 4] = matTextures[4]->GetSRV();
		m_srvs[mIndex * 6 + 5] = matTextures[5]->GetSRV();*/
	}
}

Model::Mesh& Model::GetMesh(int index)
{
	return m_pMesh[index];
}
