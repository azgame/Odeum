#include "RenderComponent.h"

#include "../Rendering/DirectX12/SceneGraph.h"

void RenderComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	isRendered = true;

	SetColour(Colour(1.0, 1.0, 1.0, 0.0));
}

void RenderComponent::OnDetach()
{
	SceneGraph::Get()->RemoveRenderObject(this);
}

void RenderComponent::Update(float deltaTime)
{
	for (int i = 0; i < m_model.kNumAnims; i++)
	{
		if (m_poses.size() == 0)
		{
			m_poses.resize(m_model.kNumAnims);
		}

		if (m_poses[i].size() == 0)
			m_poses[i].resize(m_model.m_animations[i].m_boneCount, Matrix4(kIdentity)); // use this for no animation

		if (isAnimation && totalTime > 1.0)
		{
			// maybe change this --------------------------------------------------------------- vv was m_gameObject->GetTransform()
			Animator::GetPose(m_model.m_animations[i], m_model.m_skeletons[i], totalTime, m_poses[i], Matrix4(kIdentity), globalInverseTransform);
		}
	}

	totalTime += deltaTime;
}

void RenderComponent::LoadModelFromFile(std::string fileName, bool animate)
{
	m_model.SetParent(this);
	m_model.m_details.fileName = fileName;

	SceneGraph::Get()->AddRenderObject(this);
	isAnimation = animate;
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
