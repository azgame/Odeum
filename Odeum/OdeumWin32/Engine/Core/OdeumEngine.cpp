#include "OdeumEngine.h"
#include "Debug.h"



std::unique_ptr<OdeumEngine> OdeumEngine::engineInstance = nullptr;

OdeumEngine::OdeumEngine()
{
	m_mainCamera = nullptr;
	m_renderer = nullptr;
	
	// For now hardcode number of objects, this will be passed from the game later
	m_renderObjects.push_back(new Model());
	m_renderObjects.push_back(new Model());

	m_gameInterface = nullptr;

	m_pitch = m_yaw = 0;

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
	// Initialize camera
	m_mainCamera = new Camera();
	if (!m_mainCamera) return false;

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd, m_renderObjects)) return false;

	// Initialize window size dependent resources CreateWindowSizeDependentResources(screenHeight, screenWidth, camera);
	m_renderer->CreateRasterWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);
	m_renderer->CreateRaytracingWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);

	if (!m_gameInterface->Initialize()) return false;
	m_currentSceneNum = 1;

	timer.Initialize();

	return true;
}

bool OdeumEngine::Run() 
{
	timer.UpdateFrameTicks();
	m_mainCamera->UpdateCamera();

	DirectX::XMFLOAT4 pos = m_renderObjects[0]->GetPosition();
	pos.x = 5.0f;

	m_renderObjects[0]->SetPosition(pos);
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat4(&m_renderObjects[0]->GetPosition());
	m_renderObjects[0]->m_modelMatrix = DirectX::XMMatrixTranslationFromVector(posVec);

	posVec = DirectX::XMLoadFloat4(&m_renderObjects[1]->GetPosition());
	m_renderObjects[1]->m_modelMatrix = DirectX::XMMatrixTranslationFromVector(posVec);
	
	m_gameInterface->Update(timer.GetDeltaTime());

	if (!m_renderer->Render(m_renderObjects)) return false;

	return true;
}

void OdeumEngine::Uninitialize() 
{
	delete m_gameInterface; m_gameInterface = nullptr;
	m_renderObjects.clear();
	delete m_renderer; m_renderer = nullptr;
	delete m_mainCamera; m_mainCamera = nullptr;
}
