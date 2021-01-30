#include "GGJTestScene.h"

#include "../../Engine/Math/Plane.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

GGJTestScene::GGJTestScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	for (int i = 0; i < 4; i++)
	{
		gameObjects.push_back(new GameObject());
		gameObjects[i]->AddComponent<Rigidbody>();
		gameObjects[i]->GetComponent<Rigidbody>()->SetPosition(Vector4(i * 3, 0.0f, i * 4, 1.0f));
		gameObjects[i]->AddComponent<RenderComponent>();
		gameObjects[i]->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.2, 0.2));
	}
}

GGJTestScene::~GGJTestScene()
{
}

bool GGJTestScene::Initialize()
{
	return true;
}

void GGJTestScene::Update(const float deltaTime_)
{
}

void GGJTestScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	ImGui::End();
}
