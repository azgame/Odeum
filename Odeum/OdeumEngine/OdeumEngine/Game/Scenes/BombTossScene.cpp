#include "BombTossScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"
#include "../Components/RenderComponent.h"

BombTossScene::BombTossScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	// floor (0)
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(10.0f, 0.1f, 10.0f, 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, -1.0f, 0.0f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(1.0, 1.0, 1.0));
	gameObjects.back()->SetTag("Floor");

	// p1 (0)
	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(-5.0f, 0.0f, 0.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.5f, 1.0f));
	playerObjects.back()->SetTag("Player");

	// p2 (1)
	playerObjects.push_back(new GameObject());
	playerObjects.back()->AddComponent<Rigidbody>();
	playerObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(5.0f, 0.0f, 0.0f, 0.0f));
	playerObjects.back()->AddComponent<RenderComponent>();
	playerObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.5f, 1.0f));
	playerObjects.back()->SetTag("Player");

	// bomb (1)
	gameObjects.push_back(new GameObject());
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.at(0)->GetComponent<Rigidbody>()->GetPosition().GetX(), 1.6f, playerObjects.at(0)->GetComponent<Rigidbody>()->GetPosition().GetZ(), 0.0f));
	gameObjects.back()->GetComponent<Rigidbody>()->SetScale(Vector4(0.6f, 0.6f, 0.6f, 0.0f));
	gameObjects.back()->AddComponent<RenderComponent>();
	gameObjects.back()->GetComponent<RenderComponent>()->LoadShape(ShapeTypes::CubeShape, Colour(0.0f, 0.0f, 0.0f));
	gameObjects.back()->SetTag("Bomb");
	bombTimer = 100;
}

BombTossScene::~BombTossScene()
{
}

bool BombTossScene::Initialize()
{
	return true;
}

void BombTossScene::Update(const float deltaTime_)
{
	if (Input::Get().isKeyPressed(Key::L)) {
		gameObjects.at(1)->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetX(), 1.6f, playerObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetZ(), 0.0f));
	}
	if (Input::Get().isKeyPressed(Key::A)) {
		gameObjects.at(1)->GetComponent<Rigidbody>()->SetPosition(Vector4(playerObjects.front()->GetComponent<Rigidbody>()->GetPosition().GetX(), 1.6f, playerObjects.front()->GetComponent<Rigidbody>()->GetPosition().GetZ(), 0.0f));
	}

	if (bombTimer == 0) {

	}
	else {
		bombTimer--;
	}
	//OdeumEngine::Get().GetCamera().SetPosition(Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()) + Vector3(0.0f, 10.0f, -25.0f));
	/*OdeumEngine::Get().GetCamera().SetLookDirection(
		Vector3(gameObjects.back()->GetComponent<Rigidbody>()->GetPosition()),
		Vector3(kYUnitVector)
	);*/
}

void BombTossScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");

	if (gameObjects.back()->GetComponent<Rigidbody>()->GetPosition().GetY() > 25.0f)
	{
		ImGui::Text("You win!");
	}

	ImGui::End();
}
