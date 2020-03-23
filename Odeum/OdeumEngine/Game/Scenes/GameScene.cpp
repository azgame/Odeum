#include "GameScene.h"



GameScene::GameScene() : Scene()
{
	// For now hardcode number of objects, this will be passed from the game later
	Model* model = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl");
	Model* model2 = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl");
	m_renderObjects.push_back(new GameObject(model));
	m_renderObjects.push_back(new GameObject(model2));

	model = nullptr; model2 = nullptr;
}

GameScene::~GameScene()
{
	for (auto m : m_renderObjects) SAFE_DELETE(m);
}

bool GameScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);

	m_renderObjects[0]->position = DirectX::XMFLOAT4(0.0f, 0.0f, -10.0f, 0.0f);
	m_renderObjects[1]->position = DirectX::XMFLOAT4(0.0f, 0.0f, -10.0f, 0.0f);

	m_renderObjects[0]->GetModel()->UpdateInstance(0, m_renderObjects[0]->position, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));
	m_renderObjects[1]->GetModel()->UpdateInstance(0, m_renderObjects[0]->position, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));

	return true;
}

void GameScene::Update(const float deltaTime_)
{
	DirectX::XMFLOAT4 pos = m_renderObjects[1]->position;
	m_renderObjects[1]->position = pos;
	pos = m_renderObjects[1]->position;
	pos.x -= 0.01f;
	m_renderObjects[1]->position = pos;
	m_renderObjects[1]->GetModel()->UpdateInstance(0, pos, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));
}

void GameScene::Render()
{
}
