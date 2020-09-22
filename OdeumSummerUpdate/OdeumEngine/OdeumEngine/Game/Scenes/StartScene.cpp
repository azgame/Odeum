#include "StartScene.h"


StartScene::StartScene() : Scene()
{
}

StartScene::~StartScene()
{
}

bool StartScene::Initialize()
{
	Debug::Info("Creating Start Scene", __FILENAME__, __LINE__);
	return true;
}

void StartScene::Update(const float deltaTime_)
{
}

void StartScene::Render()
{
}