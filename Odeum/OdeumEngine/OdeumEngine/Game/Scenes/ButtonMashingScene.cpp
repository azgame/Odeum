#include "ButtonMashingScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

ButtonMashingScene::ButtonMashingScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 0.2, 0.2));
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(kZero));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 0.2));
}

ButtonMashingScene::~ButtonMashingScene()
{
}

bool ButtonMashingScene::Initialize()
{
	return true;
}

void ButtonMashingScene::Update(const float deltaTime_)
{
	if (Input::Get().isKeyPressed(Key::K))
	{
		gameObjects.back()->GetComponent<Rigidbody>()->ApplyForce(Vector4(kYUnitVector));
	}

	//OdeumEngine::Get().GetCamera().SetPosition(Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()) + Vector3(0.0f, 10.0f, -25.0f));
	/*OdeumEngine::Get().GetCamera().SetLookDirection( 
		Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()), 
		Vector3(kYUnitVector)
	);*/
}

void ButtonMashingScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");

	if (gameObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetY() > 25.0f)
	{
		ImGui::Text("You win!");
	}

	ImGui::End();
}
