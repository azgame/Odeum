#include "Model.h"



Model::Model(const std::string& objFilePath_, const std::string& mtlFilePath_) : m_subMeshes(std::vector<Mesh*>())
{
	m_subMeshes.reserve(10);
	m_modelInstances.reserve(5);
	obj = new LoadOBJModel();
	obj->LoadModel(objFilePath_, mtlFilePath_);
	this->LoadModel();
}


Model::~Model()
{
	if (m_subMeshes.size() > 0) {
		for (auto m : m_subMeshes)
		{
			SAFE_DELETE(m);
		}
	}
}

bool Model::Initialize(ID3D12Device *device_, ID3D12GraphicsCommandList *commandList_)
{
	for (auto m : m_subMeshes) {
		if (m) {
			if (!m->Initialize(device_, commandList_)) return false;
		}
	}
	return true;
}

void Model::Render(ID3D12GraphicsCommandList* commandList_)
{
	for (auto m : m_subMeshes) {
		m->Render(commandList_);
	}
}

void Model::AddMesh(Mesh * mesh_)
{
	m_subMeshes.push_back(mesh_);
}

DirectX::XMMATRIX Model::GetTransform(int index_) const
{
	return m_modelInstances[index_];
}

int Model::CreateInstance(DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_)
{
	m_modelInstances.push_back(GetTransform(pos_, angle_, rot_, scale_));
	return m_modelInstances.size() - 1;
}

void Model::UpdateInstance(int index_, DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_)
{
	m_modelInstances[index_] = GetTransform(pos_, angle_, rot_, scale_);
}

void Model::UpdateInstance(int index_, DirectX::XMFLOAT4 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_)
{
	m_modelInstances[index_] = GetTransform(pos_, angle_, rot_, scale_);
}

DirectX::XMMATRIX Model::GetTransform(DirectX::XMFLOAT3 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_) 
{
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat3(&pos_);
	DirectX::XMVECTOR rotVec = DirectX::XMLoadFloat3(&rot_);
	DirectX::XMVECTOR sclVec = DirectX::XMLoadFloat3(&scale_);
	return DirectX::XMMatrixTranslationFromVector(posVec) * DirectX::XMMatrixRotationAxis(rotVec, angle_) * DirectX::XMMatrixScalingFromVector(sclVec);
}

DirectX::XMMATRIX Model::GetTransform(DirectX::XMFLOAT4 pos_, float angle_, DirectX::XMFLOAT3 rot_, DirectX::XMFLOAT3 scale_)
{
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat4(&pos_);
	DirectX::XMVECTOR rotVec = DirectX::XMLoadFloat3(&rot_);
	DirectX::XMVECTOR sclVec = DirectX::XMLoadFloat3(&scale_);
	return DirectX::XMMatrixTranslationFromVector(posVec) * DirectX::XMMatrixRotationAxis(rotVec, angle_) * DirectX::XMMatrixScalingFromVector(sclVec);
}

void Model::LoadModel()
{
	for (int i = 0; i < obj->GetSubMeshes().size(); i++)
	{
		m_subMeshes.push_back(new Mesh(obj->GetSubMeshes()[i]));
	}

	SAFE_DELETE(obj);
}
