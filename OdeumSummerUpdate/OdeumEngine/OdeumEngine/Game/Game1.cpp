#include "Game1.h"



Game1::Game1() : GameInterface()
{
}

Game1::~Game1()
{
}

bool Game1::Initialize()
{
	if (OdeumEngine::GetInstance()->GetCurrentScene() == 0)
	{
		m_currentScene = new StartScene();
		m_currentSceneNum = 0;
		return m_currentScene->Initialize();
	}

	Debug::FatalError("Engine's scene is not initialized to 0", __FILENAME__, __LINE__);
	return false;
}

void Game1::Update(const float deltaTime_)
{
	if (m_currentSceneNum != OdeumEngine::GetInstance()->GetCurrentScene())
	{
		BuildScene();
	}

	m_currentScene->Update(deltaTime_);
}

void Game1::Render()
{
}

void Game1::BuildScene()
{
	// cleanup previous scene
	delete m_currentScene;
	m_currentScene = nullptr;

	// Change to enum or queue of scenes
	switch (OdeumEngine::GetInstance()->GetCurrentScene())
	{
	case 1:
		m_currentScene = new GameScene();
		break;
	default: // case 0
		m_currentScene = new StartScene();
		break;
	}

	m_currentSceneNum = OdeumEngine::GetInstance()->GetCurrentScene();
	
	if(!m_currentScene->Initialize()) Debug::FatalError("Could not change scenes", __FILENAME__, __LINE__);
}
