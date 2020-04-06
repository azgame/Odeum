#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "LoadOBJModel.h"
#include "ShaderHandler.h"

class Model
{
public:
	Model(const std::string& objFilePath_, const std::string& mtlFilePath_, DxShaderProgram* shaderProgram_);
	~Model();
	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Render(ID3D12GraphicsCommandList* commandList_);
	void AddMesh(Mesh* mesh_);
	Mesh* GetMesh(int n = 0) { return m_subMeshes[n]; }
	DirectX::XMMATRIX GetTransform(int index_ = 0) const;

	int CreateInstance(DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_);
	void UpdateInstance(int index_, DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_);
	void UpdateInstance(int index_, DirectX::XMFLOAT4 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_);

	DxShaderProgram* GetShaderProgram() { return m_shaderProgram; }
	BoundingBox GetBoundingBox()		{ return m_box; }

private:
	std::vector<Mesh*>						m_subMeshes;
	std::vector<DirectX::XMMATRIX>			m_modelInstances;
	BoundingBox								m_box;
	LoadOBJModel*							obj;
	DxShaderProgram*						m_shaderProgram;

	DirectX::XMMATRIX GetTransform(DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_);
	DirectX::XMMATRIX GetTransform(DirectX::XMFLOAT4 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_);
	void LoadModel();
};

#endif