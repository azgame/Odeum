#include "OdeumEngine.h"
#include "Debug.h"


std::unique_ptr<OdeumEngine> OdeumEngine::engineInstance = nullptr;

OdeumEngine::OdeumEngine()
{
	m_mainCamera = nullptr;
	m_renderer = nullptr;
	
	// For now hardcode number of objects, this will be passed from the game later
	Model* model = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl");
	Model* model2 = new Model("Engine/Resources/Models/Apple.obj", "Engine/Resources/Materials/Apple.mtl");
	m_renderObjects.push_back(new GameObject(model));
	m_renderObjects.push_back(new GameObject(model2));

	m_gameInterface = nullptr;

	m_pitch = m_yaw = 0;

	m_currentSceneNum = 0;

	model = nullptr; model2 = nullptr;
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

	m_renderObjects[0]->position = DirectX::XMFLOAT4(0.0f, 0.0f, -10.0f, 0.0f);
	m_renderObjects[1]->position = DirectX::XMFLOAT4(0.0f, 0.0f, -10.0f, 0.0f);

	m_renderObjects[0]->GetModel()->UpdateInstance(0, m_renderObjects[0]->position, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));
	m_renderObjects[1]->GetModel()->UpdateInstance(0, m_renderObjects[0]->position, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd, &m_renderObjects)) return false;

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

	DirectX::XMFLOAT4 pos = m_renderObjects[1]->position;
	m_renderObjects[1]->position = pos;
	pos = m_renderObjects[1]->position;
	pos.x -= 0.01f;
	m_renderObjects[1]->position = pos;
	m_renderObjects[1]->GetModel()->UpdateInstance(0, pos, 0, DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0.5, 0.5, 0.5));
	
	m_gameInterface->Update(timer.GetDeltaTime());

	if (!m_renderer->Render()) return false;
	m_gameInterface->Render();

	return true;
}

void OdeumEngine::Uninitialize() 
{
	SAFE_DELETE(m_gameInterface);
	for (auto m : m_renderObjects) SAFE_DELETE(m);
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_mainCamera);
}
