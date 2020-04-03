#include "GameObject.h"



GameObject::GameObject(Model* model_) : m_model(nullptr)
{
	m_model = model_;
	m_model->CreateInstance(DirectX::XMFLOAT3(), 0.0f, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(1, 1, 1));
	position = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
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

void GameObject::Update(float deltaTime)
{
}

void GameObject::Render(ID3D12GraphicsCommandList * commandList_)
{
	if (m_model) {
		m_model->Render(commandList_);
	}
}
