#include "GameScene.h"
#include "../Engine/Rendering/SceneGraph.h"
#include "../Engine/Math/CollisionHandler.h"


GameScene::GameScene() : Scene()
{
	CollisionHandler::GetInstance()->Initialize(100.0f);

	// For now hardcode number of objects, this will be passed from the game later
	Model* model = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader"));
	Model* model2 = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader"));

	SceneGraph::GetInstance()->AddModel(model);
	SceneGraph::GetInstance()->AddModel(model2);

	SceneGraph::GetInstance()->AddGameObject(new GameObject((model), DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f)));
	SceneGraph::GetInstance()->AddGameObject(new GameObject((model2), DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f)));

	model = nullptr; model2 = nullptr;
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
	SceneGraph::GetInstance()->GetGameObject("GameObject1")->position.x += SceneGraph::GetInstance()->GetGameObject("GameObject1")->movX; // Testing mouse picking

	SceneGraph::GetInstance()->Update(deltaTime_);
}

void GameScene::Render()
{
}
