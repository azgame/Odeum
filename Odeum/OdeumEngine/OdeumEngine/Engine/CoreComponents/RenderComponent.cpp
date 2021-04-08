#include "RenderComponent.h"

#include "../Rendering/DirectX12/SceneGraph.h"

void RenderComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	isAnimation = false;
	isRendered = true;
	currentPose = {};
	globalInverseTransform = ~Matrix4(kIdentity); // ~ is used for inverse
	//globalInverseTransform = Matrix4(DirectX::XMMatrixInverse(nullptr, globalInverseTransform));
	totalTime = 0.0f;
}

void RenderComponent::OnDetach()
{
	SceneGraph::Get()->RemoveRenderObject(this);
}

void RenderComponent::Update(float deltaTime)
{
	if (isAnimation)
	{
		
		currentPose.resize(m_model.m_boneCount, Matrix4(kIdentity)); // use this for no animation
		if (totalTime > 1.0f)
			Animator::GetPose(m_model.animation, m_model.m_skeleton, totalTime, currentPose, m_gameObject->GetTransform(), globalInverseTransform);
	}
	totalTime += deltaTime;
}

void RenderComponent::LoadModelFromFile(std::string fileName)
{
	m_model.SetParent(this);
	m_model.m_details.fileName = fileName;

	SceneGraph::Get()->AddRenderObject(this);
	isAnimation = false;
}

void RenderComponent::LoadModelFromFile(std::string fileName, bool animate)
{
	m_model.SetParent(this);
	m_model.m_details.fileName = fileName;

	SceneGraph::Get()->AddRenderObject(this);
	isAnimation = true;
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
