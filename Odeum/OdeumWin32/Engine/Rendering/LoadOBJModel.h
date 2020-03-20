#ifndef LOADOBJMODEL_H
#define LOADOBJMODEL_H

#include "../../pch.h"
#include "Mesh.h"
#include "TextureHandler.h"
#include <sstream>

class LoadOBJModel
{
public:
	LoadOBJModel();
	~LoadOBJModel();

	void LoadModel(const std::string& objFilePath_, const std::string& mtlFilePath_);
	void LoadModel(const std::string& filePath_);

	std::vector<Vertex> GetVerts();
	std::vector<UINT16> GetIndices();
	std::vector<SubMesh> GetSubMeshes();

	void Shutdown();

private:
	std::vector<DirectX::XMFLOAT3>		vertices;
	std::vector<DirectX::XMFLOAT3>		normals;
	std::vector<DirectX::XMFLOAT2>		textureCoords;
	std::vector<UINT16>					indices, posIndices, normalIndices, textureIndices;
	std::vector<Vertex>					meshVertices;
	std::vector<SubMesh>				subMeshes;
	int									totalVerts;

	void PostProcessing();
	void LoadMaterial(const std::string& matName_);
	void LoadMaterialLibrary(const std::string& matName_);

};

#endif