#include "GameScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../../Engine/Math/CollisionHandler.h"
#include "../Components/Rigidbody.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject("Engine/Resources/Models/Cottage_FREE.obj");
	object->AddComponent<Rigidbody>();

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));
}

GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
	LOG("Creating Game Scene")
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	angle += direction * (deltaTime_ * 0.1f);

	object->GetComponent<Rigidbody>()->SetRotation(Quaternion(Vector3(kYUnitVector), angle));
	object->Update(deltaTime_);
}

void GameScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Change rotation direction"))
		direction *= -1.0f;
	ImGui::End();
}
