#include "GameObject.h"



GameObject::GameObject(Model* model_) : m_model(nullptr)
{
	m_model = model_;
}


GameObject::~GameObject()
{
	SAFE_DELETE(m_model);
}

bool GameObject::Initialize(ID3D12Device *device_, ID3D12GraphicsCommandList *commandList_)
{
	if (!m_model->Initialize(device_, commandList_)) return false;
	return true;
}

void GameObject::Render(ID3D12GraphicsCommandList * commandList_)
{
	if (m_model) {
		m_model->Render(commandList_);
	}
}
