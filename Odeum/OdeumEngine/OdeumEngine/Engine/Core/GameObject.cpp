#include "GameObject.h"

#include "../Rendering/DirectX12/SceneGraph.h"
#include "Component.h"

// Moving model loading to a graphics component
GameObject::GameObject(std::string fileName)
{
	m_model.Load(fileName);

	m_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotation = Vector4(kYUnitVector);
	m_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);

	SceneGraph::Get()->AddGameObject(this);
}

GameObject::GameObject(ShapeTypes preDefinedShape, Colour colour)
{
	switch (preDefinedShape)
	{
	case ShapeTypes::CubeShape:
	{
		m_model.Load("Engine/Resources/Models/Cube.obj");
		break;
	}
	default:
		break;
	}
}

GameObject::~GameObject()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}

	m_components.clear();
}

void GameObject::Initialize(std::string modelTextureLoadFile)
{
	fileName = modelTextureLoadFile;
}

void GameObject::Update(float deltaTime)
{
	for (auto component : m_components)
		component->Update(deltaTime);
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

void GameObject::SetMass(float mass)
{
	m_mass = mass;
}

void GameObject::CreateAttachedComponent(Component* pAttachedComponent)
{
	// Attach T
	pAttachedComponent->OnAttach(this); // On attach allows implementations to define how they register to systems.  
											// eg. A component which inherits from GraphicsComponent will register with the scene graph
	m_components.push_back(pAttachedComponent);
}

void GameObject::UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale)
{
	m_modelMatrix = Matrix4(DirectX::XMMatrixTranslationFromVector(position.GetVec()) * DirectX::XMMatrixRotationAxis(rotation.GetVec(), angle) * DirectX::XMMatrixScalingFromVector(scale.GetVec()));	
}
