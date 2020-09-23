#include "Game1.h"


Game1::Game1() : GameInterface()
{
}

Game1::~Game1()
{
}

bool Game1::Initialize()
{
	ASSERT(OdeumEngine::Get().GetCurrentScene() == 0, "Engine's default scene is not 0");

	m_currentScene = new StartScene();
	m_currentSceneNum = 0;
	return m_currentScene->Initialize();
}

void Game1::Update(const float deltaTime_)
{
	if (m_currentSceneNum != OdeumEngine::Get().GetCurrentScene())
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
	delete m_currentScene;
	m_currentScene = nullptr;

	ASSERT(OdeumEngine::Get().GetCurrentScene() >= 0, "Current Scene index set to -1");

	switch (OdeumEngine::Get().GetCurrentScene())
	{
	case 1:
		m_currentScene = new GameScene();
		break;
	case 2:
		m_currentScene = new RobScene();
	default: // case 0
		m_currentScene = new StartScene();
		break;
	}

	m_currentSceneNum = OdeumEngine::Get().GetCurrentScene();
	
	if(!m_currentScene->Initialize()) Debug::FatalError("Could not change scenes", __FILENAME__, __LINE__);
}
