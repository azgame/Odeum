#include "Game1.h"

#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"
#include "Scenes/RobScene.h"
#include "Scenes/BenScene.h"
#include "Scenes/GGJTestScene.h"
#include "Scenes/ButtonMashingScene.h"
#include "Scenes/BombTossScene.h"
#include "Scenes/QuickDrawScene.h"
#include "Scenes/BattleScene.h"
#include "Components/StatComponent.h"
#include "../Engine/Rendering/DirectX12/SceneGraph.h"
#include "../Engine/Core/SceneObjects.h"
#include "Scenes/counting.h"
#include "Scenes/MultiTaskingScene.h"
#include "Scenes/CollectingScene.h"
#include "Scenes/DodgeScene.h"
#include "Scenes/DodgeFlyScene.h"
#include "Scenes/Collecting3DScene.h"
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

	SceneGraph::Get()->LoadGraphicsObjects();

	SceneObjects::Get()->UpdateObjects(deltaTime_);
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

	SceneObjects::Get()->UnloadCurrentScene();

	m_currentSceneNum = OdeumEngine::Get().GetCurrentScene();
	SceneObjects::Get()->SetCurrentScene(m_currentSceneNum);

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
		m_currentScene = new GGJTestScene();
		break;
	case 5:
		m_currentScene = new ButtonMashingScene();
		break;
	case 6:
		m_currentScene = new BombTossScene();
		break;
	case 7:
		m_currentScene = new QuickDrawScene();
		break;
	case 8:
		m_currentScene = new Counting();
		break;
	case 9:
		m_currentScene = new BattleScene();
		break;
	case 10:
		m_currentScene = new MultiTaskingScene();
		break;
	case 11:
		m_currentScene = new CollectingScene();
		break;
	case 12:
		m_currentScene = new DodgeScene();
		break;
	case 13:
		m_currentScene = new DodgeFlyScene();
		break;
	case 14:
		m_currentScene = new Collecting3DScene();
		break;
	default: // case 0
		m_currentScene = new StartScene();
		break;
	}

	if(!m_currentScene->Initialize()) Debug::FatalError("Could not change scenes", __FILENAME__, __LINE__);

	SceneObjects::Get()->InitializeObjects();
	SceneGraph::Get()->LoadGraphicsObjects();
	SceneObjects::Get()->UpdateObjects(0.0167f);
}
