#include "GameScene.h"



GameScene::GameScene() : Scene()
{
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
}

void GameScene::Render()
{
}
