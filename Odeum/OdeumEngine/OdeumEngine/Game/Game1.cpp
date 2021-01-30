#include "Game1.h"

#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"
#include "Scenes/RobScene.h"
#include "Scenes/BenScene.h"
#include "Scenes/GJKTestScene.h"
#include "Scenes/PhysicsTestScene.h"
#include "Scenes/PhysicsTestScene2.h"
#include "Scenes/GGJTestScene.h"
#include "Scenes/ButtonMashingScene.h"

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

void Game1::UIRender()
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
		m_currentScene = new GJKTestScene();
		break;
	case 5:
		m_currentScene = new PhysicsTestScene();
		break;
	case 6:
		m_currentScene = new PhysicsTestScene2();
		break;
	case 7:
		m_currentScene = new GGJTestScene();
		break;
	case 8:
		m_currentScene = new ButtonMashingScene();
		break;
	default: // case 0
		m_currentScene = new StartScene();
		break;
	}

	m_currentSceneNum = OdeumEngine::Get().GetCurrentScene();
	
	if(!m_currentScene->Initialize()) Debug::FatalError("Could not change scenes", __FILENAME__, __LINE__);
}
