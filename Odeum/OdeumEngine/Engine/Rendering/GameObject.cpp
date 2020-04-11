#include "GameObject.h"

#include "../Math/CollisionHandler.h"

GameObject::GameObject(Model* model_, DirectX::XMFLOAT3 pos_) : m_model(nullptr)
{
	m_model = model_;
	m_modelInstance = -1;
	position = pos_;
	angle = 0.0f;
	rotation = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	if (m_model)
	{
		m_modelInstance = m_model->CreateInstance(position, angle, rotation, scale);
		m_box = m_model->GetBoundingBox();
		m_box.transform = m_model->GetTransform(m_modelInstance);
	}

	movX = 0.01f;
	hit = false;
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
	m_model->UpdateInstance(m_modelInstance, position, angle, rotation, scale);
	m_box.transform = m_model->GetTransform(m_modelInstance);
}

void GameObject::Render(ID3D12GraphicsCommandList * commandList_)
{
	if (m_model) {
		m_model->Render(commandList_);
	}
}

void GameObject::SetHit(bool hit_, DirectX::Mouse::State mouse_)
{
	hit = hit_;
	if (hit)
		movX = -movX;
}

void GameObject::SetPosition(DirectX::XMFLOAT3 position_)
{
	position = position_;
	if (m_model) {
		m_model->UpdateInstance(m_modelInstance, position, angle, rotation, scale);
		m_box.transform = m_model->GetTransform(m_modelInstance);
	}
}

void GameObject::SetAngle(float angle_)
{
	angle = angle_;
	if (m_model) {
		m_model->UpdateInstance(m_modelInstance, position, angle, rotation, scale);
		m_box.transform = m_model->GetTransform(m_modelInstance);
	}
}

void GameObject::SetRotation(DirectX::XMFLOAT3 rotation_)
{
	rotation = rotation_;
	if (m_model) {
		m_model->UpdateInstance(m_modelInstance, position, angle, rotation, scale);
		m_box.transform = m_model->GetTransform(m_modelInstance);
	}
}

void GameObject::SetScale(DirectX::XMFLOAT3 scale_)
{
	scale = scale_;
	if (m_model) {
		m_model->UpdateInstance(m_modelInstance, position, angle, rotation, scale);
		m_box.transform = m_model->GetTransform(m_modelInstance);
	}
}
