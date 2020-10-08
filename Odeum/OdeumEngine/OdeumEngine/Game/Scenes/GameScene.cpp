#include "GameScene.h"
#include "../Components/KinimaticMovement.h"
#include "../Components/DynamicMovement.h"
#include "../Components/ComponentTest.h"

GameScene::GameScene() : Scene()
{
	object = new GameObject("Engine/Resources/Models/cottage_obj.obj");

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(-10.0f, 10.0f, 50.0f));
	object->AddComponents<DynamicMovement, KinimaticMovement, ComponentTest>();
	object->RemoveComponent<KinimaticMovement>();
	object->RemoveComponents<DynamicMovement, ComponentTest>();
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

	angle += deltaTime_ * 0.1f;
	object->SetRotation(Vector4(kYUnitVector), angle);

	object->Update(deltaTime_);
}

void GameScene::Render()
{
}
