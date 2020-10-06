#include "GameScene.h"

GameScene::GameScene() : Scene()
{
	object = new GameObject("Engine/Resources/Models/Apple.obj");

	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 5.0f, 25.0f));
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

	angle += 0.0025f;
	object->SetRotation(Vector4(0.0f, 1.0f, 0.0f, 0.0f), angle);
}

void GameScene::Render()
{
}
