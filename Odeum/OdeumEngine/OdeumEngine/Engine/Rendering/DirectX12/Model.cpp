#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../CoreComponents/RenderComponent.h"
#include "../../Animation/Animator.h"

#include <thread>

void Model::Load(bool isAnimation)
{
	std::string fileName = m_details.fileName;

	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
		aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 0xFFFFFFFF);
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 0xFFFE);

	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

	const aiScene* scene = importer.ReadFile(fileName.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		//aiProcess_RemoveComponent |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ValidateDataStructure |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_SortByPType |
		//aiProcess_FindInvalidData |
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
	m_pMaterialDetails = new MaterialInfo[m_details.materialCount];
	memset(m_pMaterialDetails, 0, sizeof(MaterialInfo) * m_details.materialCount);

	for (unsigned int mIndex = 0; mIndex < scene->mNumMaterials; mIndex++)
	{
		const aiMaterial* sourceMat = scene->mMaterials[mIndex];
		Material& destMat = m_pMaterials[mIndex];
		MaterialInfo& destMatInfo = m_pMaterialDetails[mIndex];

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

		destMat.flatColour = Vector4(m_flatColour);
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

		strncpy_s(destMatInfo.name, matName.C_Str(), 127);
		strncpy_s(destMatInfo.diffuseTextureFile, texDiffusePath.C_Str(), 127);
		strncpy_s(destMatInfo.specularTextureFile, texSpecularPath.C_Str(), 127);
		strncpy_s(destMatInfo.emissiveTextureFile, texEmissivePath.C_Str(), 127);
		strncpy_s(destMatInfo.normalTextureFile, texNormalPath.C_Str(), 127);
		strncpy_s(destMatInfo.lightmapTextureFile, texLightmapPath.C_Str(), 127);
		strncpy_s(destMatInfo.reflectionTextureFile, texReflectionPath.C_Str(), 127);
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

	// using these vectors for now, could change after
	std::vector<Vertex> verticesOutput;
	//Vertex* verticesOutput = new Vertex[m_details.vertexCount];
	std::vector<uint32_t> indicesOutput;

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


			// bone stuff
			if (isAnimation) {
				vertex->boneIDs = DirectX::XMINT4(0, 0, 0, 0);
				vertex->boneWeights = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

				// try and add bone stuff here
				/*m_boneInfo = {};
				std::vector<uint32_t> boneCounts;
				boneCounts.resize(sourceMesh->mNumVertices, 0);
				m_boneCount = sourceMesh->mNumBones;

				// loop through each bone
				for (unsigned int i = 0; i < m_boneCount; i++)
				{
					aiBone* bone = sourceMesh->mBones[i];
					Matrix4 matrix = Math::ASSIMPToMatrix4(bone->mOffsetMatrix);
					m_boneInfo[bone->mName.C_Str()] = { i, matrix };

					// loop through each vertex that has that bone
					for (int j = 0; j < bone->mNumWeights; j++) {
						uint32_t id = bone->mWeights[j].mVertexId;
						float weight = bone->mWeights[j].mWeight;
						boneCounts[id]++;

						switch (boneCounts[id])
						{
						case 1:
							verticesOutput[id].boneIDs.x = i;
							verticesOutput[id].boneWeights.SetX(weight);
							break;
						case 2:
							verticesOutput[id].boneIDs.y = i;
							verticesOutput[id].boneWeights.SetY(weight);
							break;
						case 3:
							verticesOutput[id].boneIDs.z = i;
							verticesOutput[id].boneWeights.SetZ(weight);
							break;
						case 4:
							verticesOutput[id].boneIDs.w = i;
							verticesOutput[id].boneWeights.SetW(weight);
							break;
						default:
							Debug::Error("Unable to allocate bone to vertex.", "Model.cpp", __LINE__);
							break;
						}
					}

				}

				// normalize weights to make all weight sum 1
				for (int i = 0; i < verticesOutput.size(); i++)
				{
					float totalWeight = verticesOutput[i].boneWeights.GetX() + verticesOutput[i].boneWeights.GetY() + verticesOutput[i].boneWeights.GetZ() + verticesOutput[i].boneWeights.GetW();
					if (totalWeight > 0.0f)
					{
						verticesOutput[i].boneWeights.Normalize();
					}
				}
				*/
				verticesOutput.push_back(*vertex);
			}


			vertex++;
		}

		// more bone stuff
		// load bone data to vertices
		if (isAnimation)
		{
			// this info should be right???
			m_boneInfo = {};
			std::vector<uint32_t> boneCounts;
			boneCounts.resize(sourceMesh->mNumVertices, 0);
			m_boneCount = sourceMesh->mNumBones;

			// loop through each bone
			for (unsigned int i = 0; i < m_boneCount; i++)
			{
				aiBone* bone = sourceMesh->mBones[i];
				Matrix4 matrix = Math::ASSIMPToMatrix4(bone->mOffsetMatrix);
				m_boneInfo[bone->mName.C_Str()] = { i, matrix };

				// loop through each vertex that has that bone
				for (int j = 0; j < bone->mNumWeights; j++) {
					uint32_t id = bone->mWeights[j].mVertexId;
					float weight = bone->mWeights[j].mWeight;
					boneCounts[id]++;

					switch (boneCounts[id])
					{
					case 1:
						m_pVertexData[id].boneIDs.x = i;
						m_pVertexData[id].boneWeights.SetX(weight);
						animation.boneIDs[id].SetX(i);
						animation.boneWeights[id].SetX(weight);
						//verticesOutput[id].boneIDs.x = i;
						//verticesOutput[id].boneWeights.SetX(weight);
						break;
					case 2:
						m_pVertexData[id].boneIDs.y = i;
						m_pVertexData[id].boneWeights.SetY(weight);
						animation.boneIDs[id].SetY(i);
						animation.boneWeights[id].SetY(weight);
						//verticesOutput[id].boneIDs.y = i;
						//verticesOutput[id].boneWeights.SetY(weight);
						break;
					case 3:
						m_pVertexData[id].boneIDs.z = i;
						m_pVertexData[id].boneWeights.SetZ(weight);
						animation.boneIDs[id].SetZ(i);
						animation.boneWeights[id].SetZ(weight);
						//verticesOutput[id].boneIDs.z = i;
						//verticesOutput[id].boneWeights.SetZ(weight);
						break;
					case 4:
						m_pVertexData[id].boneIDs.w = i;
						m_pVertexData[id].boneWeights.SetW(weight);
						animation.boneIDs[id].SetW(i);
						animation.boneWeights[id].SetW(weight);
						//verticesOutput[id].boneIDs.w = i;
						//verticesOutput[id].boneWeights.SetW(weight);
						break;
					default:
						Debug::Error("Unable to allocate bone to vertex.", "Model.cpp", __LINE__);
						break;
					}
				}
			}

			// normalize weights to make all weight sum 1
			for (int i = 0; i < verticesOutput.size(); i++)
			{
				float totalWeight = m_pVertexData[i].boneWeights.GetX() + m_pVertexData[i].boneWeights.GetY() + m_pVertexData[i].boneWeights.GetZ() + m_pVertexData[i].boneWeights.GetW();
				if (totalWeight > 0.0f)
				{
					m_pVertexData[i].boneWeights.Normalize();
				}
			}


		}
		
		uint16_t* indice = m_pIndexData + destMesh.indexOffset;

		for (unsigned int fIndex = 0; fIndex < sourceMesh->mNumFaces; fIndex++)
		{
			ASSERT(sourceMesh->mFaces[fIndex].mNumIndices == 3, "Must be triangle indicies");

			*indice++ = sourceMesh->mFaces[fIndex].mIndices[0];
			*indice++ = sourceMesh->mFaces[fIndex].mIndices[1];
			*indice++ = sourceMesh->mFaces[fIndex].mIndices[2];
		}

		// might want to move this
		if (isAnimation) {
			// create bone hierarchy
			Animator::ReadSkeleton(m_skeleton, scene->mRootNode, m_boneInfo);
			Animator::LoadAnimation(scene, animation);
		}
	}
	//Vertex* b_pVertexData = &verticesOutput[0];
	
	//m_pVertexData = &verticesOutput[0];
	

	//m_vertexBuffer.Create("Vertex buffer", m_details.vertexCount, m_vertexStride, b_pVertexData);
	m_vertexBuffer.Create("Vertex buffer", m_details.vertexCount, m_vertexStride, m_pVertexData);
	m_indexBuffer.Create("Index buffer", m_details.indexCount, sizeof(uint16_t), m_pIndexData);

	LoadTextures();

	m_renderComponent->SetBoundingBox(vMin, vMax);

	isLoaded = true;
}

void Model::LoadTextures()
{
	m_srvs = new D3D12_CPU_DESCRIPTOR_HANDLE[m_details.materialCount * 6];
	const Texture* matTextures[6] = {};

	for (uint32_t mIndex = 0; mIndex < m_details.materialCount; mIndex++)
	{
		const MaterialInfo& materialInfo = m_pMaterialDetails[mIndex];

		if (!std::string(materialInfo.diffuseTextureFile).empty())
		{
			matTextures[0] = TextureManager::Get()->LoadFromFile(materialInfo.diffuseTextureFile);

			matTextures[1] = TextureManager::Get()->LoadFromFile(materialInfo.specularTextureFile);
			if (!matTextures[1]->isValid())
				matTextures[1] = matTextures[0];

			matTextures[3] = TextureManager::Get()->LoadFromFile(materialInfo.normalTextureFile);
		}
		else
		{
			matTextures[0] = TextureManager::Get()->LoadColour(Colour(-1, -1, -1, -1));
			matTextures[1] = matTextures[0];
			matTextures[3] = matTextures[0];
		}

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

Model::Material& Model::GetMaterial(int index)
{
	return m_pMaterials[index];
}
