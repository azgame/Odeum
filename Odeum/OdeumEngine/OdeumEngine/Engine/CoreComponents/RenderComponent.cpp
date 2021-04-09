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
	if (isAnimation)
	{

		currentPose.resize(m_model.m_boneCount, Matrix4(kIdentity)); // use this for no animation
		if (totalTime > 1.0f) {
			Animator::GetPose(m_model.animation, m_model.m_skeleton, totalTime, currentPose, m_gameObject->GetTransform(), globalInverseTransform);
			
			// bone transformations - was shader code
			Matrix4 bone_transforms[50];
			// not sure about these values vvvvvv
			bone_transforms[m_model.m_boneCount] = currentPose[0];
			Vector4 bw;
			// need to iterate through each vertex vv need to change the ending point i'm pretty sure
			//for (Vertex *v = m_model.m_pVertexData; m_model.m_details.vertexCount; v++)
			Vertex* v = m_model.m_pVertexData;
			for(int i = 0; i < m_model.m_details.vertexCount; i++)
			{

				bw = Vector4();
				if (int(v->boneIDs.x) == 1) {
					bw.SetZ(v->boneIDs.x);
				}

				v->boneWeights.Normalize();
				Matrix4 boneTransform = Matrix4();
				boneTransform = boneTransform + bone_transforms[int(v->boneIDs.x)] * v->boneWeights.GetX();
				boneTransform = boneTransform + bone_transforms[int(v->boneIDs.y)] * v->boneWeights.GetY();
				boneTransform = boneTransform + bone_transforms[int(v->boneIDs.z)] * v->boneWeights.GetZ();
				boneTransform = boneTransform + bone_transforms[int(v->boneIDs.w)] * v->boneWeights.GetW();

				boneTransform = boneTransform * Matrix4(DirectX::XMMatrixTranslationFromVector(Vector3(1.0f, 0.0f, 0.0f)));

				Vector4 pos = boneTransform * Vector4(v->position, 1.0f);
				// Testing movement of vertexes manually
				//pos = pos + Vector4(0.5f, 0.0f, 0.0f, 1.0f);
				Vector4 norm = boneTransform * Vector4(v->normal, 1.0f);

				v->position = DirectX::XMFLOAT3(pos.GetX(), pos.GetY(), pos.GetZ());
				v->normal = DirectX::XMFLOAT3(norm.GetX(), norm.GetY(), norm.GetZ());
				v++;
			}
			
		}
	}
	totalTime += deltaTime;
}

void RenderComponent::LoadModelFromFile(std::string fileName)
{
	m_model.SetParent(this);
	m_model.m_details.fileName = fileName;

	SceneGraph::Get()->AddRenderObject(this);
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
