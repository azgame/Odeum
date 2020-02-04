#include "Model.h"



Model::Model() : m_subMeshes(std::vector<Mesh*>())
{
	m_subMeshes.reserve(10);

	m_modelMatrix = DirectX::XMMatrixIdentity();
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
