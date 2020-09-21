#include "GameObject.h"

#include "../Rendering/DirectX12/SceneGraph.h"

GameObject::GameObject(std::string fileName, ShapeTypes preDefinedShape)
{
	if (preDefinedShape != ShapeTypes::NoShape)
	{
		switch (preDefinedShape)
		{
		case ShapeTypes::CubeShape:
			Cube cube;
			m_model.Load(&cube.GetVertices(), cube.NumVertices(), sizeof(Vertex), &cube.GetIndices(), cube.NumIndices());
		}
	}
	else // load from file
	{

	}

	m_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotation = Vector4(kYUnitVector);
	m_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);

	SceneGraph::Get()->AddGameObject(this);
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(Vector4 position)
{
	m_position = position;
	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);
}

void GameObject::SetRotation(Vector4 rotation, float angle)
{
	m_rotation = rotation;
	UpdateTransform(m_position, angle, m_rotation, m_scale);
}

void GameObject::SetScale(Vector4 scale)
{
	m_scale = scale;
	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);
}

void GameObject::UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale)
{
	m_modelMatrix = Matrix4(DirectX::XMMatrixTranslationFromVector(position.GetVec()) * DirectX::XMMatrixRotationAxis(rotation.GetVec(), angle) * DirectX::XMMatrixScalingFromVector(scale.GetVec()));
}
