#include "GameScene.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/ComponentTest.h"
#include "../../Engine/Math/CollisionHandler.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject("Engine/Resources/Models/Cottage_FREE.obj");

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, 25.0f));

	CollisionHandler::GetInstance()->Initialize(100.0f);
	CollisionHandler::GetInstance()->AddObject(object);
}

GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void GameScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	angle += direction * (deltaTime_ * 0.1f);
	object->SetRotation(Vector4(kYUnitVector), angle);

	CollisionHandler::GetInstance()->MouseUpdate();

	object->Update(deltaTime_);
}

void GameScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Change rotation direction"))
		direction *= -1.0f;
	ImGui::End();
}
