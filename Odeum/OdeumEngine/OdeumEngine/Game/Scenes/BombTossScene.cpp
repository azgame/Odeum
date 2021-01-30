#include "BombTossScene.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"

BombTossScene::BombTossScene()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));

	gameObjects.push_back(new GameObject(ShapeTypes::CubeShape, Colour(1.0, 0.2, 0.2)));
	gameObjects.push_back(new GameObject(ShapeTypes::CubeShape, Colour(1.0, 1.0, 0.2)));
	gameObjects.back()->AddComponent<Rigidbody>();
	gameObjects.back()->GetComponent<Rigidbody>()->SetPosition(Vector4(kZero));

	SceneGraph::Get()->LoadObjectsIntoMemory();
	SceneGraph::Get()->UpdateObjects(0.0167f);
}

BombTossScene::~BombTossScene()
{
}

bool BombTossScene::Initialize()
{
	return false;
}

void BombTossScene::Update(const float deltaTime_)
{
	SceneGraph::Get()->UpdateObjects(deltaTime_);

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
