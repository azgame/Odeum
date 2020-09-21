#include "GameScene.h"

GameScene::GameScene() : Scene()
{
	object = new GameObject("empty", ShapeTypes::CubeShape);
	plane = new GameObject("empty", ShapeTypes::CubeShape);
	plane->SetScale(Vector4(3.0f, 0.2f, 3.0f, 1.0f));
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
	object->SetPosition(object->GetPosition() + Vector4(0.01f, 0.0f, 0.0f, 0.0f));

	Vector3 look = OdeumEngine::Get().GetCamera().LookAt();
	OdeumEngine::Get().GetCamera().SetLookDirection(look + Vector3(0.005f, 0.0f, 0.0f));

	if (Input::Get().isKeyPressed(Key::KeyCode::A))
		Debug::Info("Key A pressed in game scene", __FILENAME__, __LINE__);
}

void GameScene::Render()
{
}
