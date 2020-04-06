#include "OdeumEngine.h"
#include "Debug.h"
#include "../Rendering/ShaderHandler.h"
#include "../Rendering/SceneGraph.h"
#include "../Math/CollisionHandler.h"

std::unique_ptr<OdeumEngine> OdeumEngine::engineInstance = nullptr;

OdeumEngine::OdeumEngine()
{
	m_mainCamera = nullptr;
	m_renderer = nullptr;
	m_gameInterface = nullptr;

	m_currentSceneNum = 0;
}


OdeumEngine::~OdeumEngine()
{
	Uninitialize();
}

OdeumEngine* OdeumEngine::GetInstance() {
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new OdeumEngine);
	}
	return engineInstance.get();
}

void OdeumEngine::SetGameInterface(GameInterface * gameInterface_)
{
	m_gameInterface = gameInterface_;
}

int OdeumEngine::GetCurrentScene()
{
	return m_currentSceneNum;
}

void OdeumEngine::SetCurrentScene(int sceneNum_)
{
	m_currentSceneNum = sceneNum_;
}

void OdeumEngine::Shutdown()
{
	// change is running to false
}

bool OdeumEngine::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{
	timer.Initialize();

	// Initialize camera
	m_mainCamera = new Camera();
	if (!m_mainCamera) return false;

	if (!m_gameInterface->Initialize()) return false;
	m_currentSceneNum = 1;
	m_gameInterface->Update(timer.GetDeltaTime());

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd, SceneGraph::GetInstance()->GetRenderModels())) return false;

	// Initialize window size dependent resources CreateWindowSizeDependentResources(screenHeight, screenWidth, camera);
	m_renderer->CreateWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);

	return true;
}

bool OdeumEngine::Run() 
{
	timer.UpdateFrameTicks();
	m_mainCamera->UpdateCamera();

	Input::GetInstance()->m_mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	DirectX::Mouse::State state = Input::GetInstance()->mouse;

	if (Input::GetInstance()->mouse.leftButton)
		CollisionHandler::GetInstance()->MouseUpdate(Input::GetInstance()->mouse); // TODO - Aidan: Break here to check state variable for mouse input. sometimes buggy

	Input::GetInstance()->m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	
	m_gameInterface->Update(timer.GetDeltaTime());

	if (!m_renderer->Render()) return false;
	m_gameInterface->Render();

	return true;
}

void OdeumEngine::Uninitialize() 
{
	SAFE_DELETE(m_gameInterface);
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_mainCamera);
}
