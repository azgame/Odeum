#include "RenderComponent.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

void RenderComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}

void RenderComponent::Update(float deltaTime)
{
}

void RenderComponent::LoadModelFromFile(std::string fileName)
{
	m_model.SetParent(this);
	m_model.m_details.fileName = fileName;

	SceneGraph::Get()->AddRenderObject(this);
}

void RenderComponent::LoadShape(ShapeTypes preDefinedShape, Colour colour)
{
	m_model.SetParent(this);

	switch (preDefinedShape)
	{
	case ShapeTypes::CubeShape:
	{
		m_model.m_details.fileName = "Engine/Resources/Models/Cube.obj";
		m_model.m_flatColour = colour;
		break;
	}
	case ShapeTypes::SphereShape:
	{
		m_model.m_details.fileName = "Engine/Resources/Models/Sphere.obj";
		m_model.m_flatColour = colour;
		break;
	}
	case ShapeTypes::PlaneShape:
	{
		break;
	}
	default:
		break;
	}

	SceneGraph::Get()->AddRenderObject(this);
}

void RenderComponent::CreateFromExistingModel(Model* Model)
{
	m_model = *Model;
	m_model.SetParent(this);

	SceneGraph::Get()->AddRenderObject(this);
}
