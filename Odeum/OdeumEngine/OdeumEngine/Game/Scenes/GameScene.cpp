#include "GameScene.h"
#include "../Components/KinematicMovement.h"
#include "../Components/DynamicMovement.h"
#include "../../Engine/Math/CollisionHandler.h"

GameScene::GameScene() : Scene(), angle(0.0f), direction(1.0f)
{
	object = new GameObject(CubeShape, Colour(0.2f, 0.0f, 0.8f));

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 25.0f));

	CollisionHandler::GetInstance()->Initialize(1000.0f);
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

	angle += direction * (deltaTime_);
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
