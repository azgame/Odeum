#include "Game1.h"

#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"
#include "Scenes/RobScene.h"
#include "Scenes/BenScene.h"
#include "Scenes/BenScene2.h"

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
	m_currentScene->UIRender();
}

void Game1::BuildScene()
{
	delete m_currentScene;
	m_currentScene = nullptr;

	ASSERT(OdeumEngine::Get().GetCurrentScene() >= 0, "Current Scene index set to a negative number!");

	switch (OdeumEngine::Get().GetCurrentScene())
	{
	case 1:
		m_currentScene = new GameScene();
		break;
	case 2:
		m_currentScene = new RobScene();
		break;
	case 3:
		m_currentScene = new BenScene();
		break;
	case 4:
		m_currentScene = new BenScene2();
		break;
	default: // case 0
		m_currentScene = new StartScene();
		break;
	}

	m_currentSceneNum = OdeumEngine::Get().GetCurrentScene();
	
	if(!m_currentScene->Initialize()) Debug::FatalError("Could not change scenes", __FILENAME__, __LINE__);
}
