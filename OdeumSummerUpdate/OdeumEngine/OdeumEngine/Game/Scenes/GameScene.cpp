#include "GameScene.h"

GameScene::GameScene() : Scene()
{
	object = new GameObject("empty", ShapeTypes::CubeShape, Colour(1.0f, 0.2f, 0.6f, 1.0f));
	object->SetPosition(Vector4(0.0f, 0.0f, 3.0f, 1.0f));

	plane = new GameObject("empty", ShapeTypes::CubeShape, Colour(0.5f, 0.5f, 0.5f, 1.0f));
	plane->SetScale(Vector4(3.0f, 0.01f, 3.0f, 1.0f));
	plane->SetPosition(Vector4(0.0f, -5.0f, 0.0f, 1.0f));
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
