#include "OdeumEngine.h"
#include "Debug.h"

const float ROTATION_GAIN = 0.004f;
const float MOVEMENT_GAIN = 0.07f;

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

	timer.Initialize();

	return true;
}

bool OdeumEngine::Run() 
{
	timer.UpdateFrameTicks();
	UpdateCamera();

	DirectX::XMFLOAT4 pos = m_renderObjects[0]->GetPosition();
	pos.x += 0.1f;

	m_renderObjects[0]->SetPosition(pos);
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat4(&m_renderObjects[0]->GetPosition());
	m_renderObjects[0]->m_modelMatrix = DirectX::XMMatrixTranslationFromVector(posVec);

	posVec = DirectX::XMLoadFloat4(&m_renderObjects[1]->GetPosition());
	m_renderObjects[1]->m_modelMatrix = DirectX::XMMatrixTranslationFromVector(posVec);
	
	if (!m_renderer->Render(m_renderObjects)) return false;
	// DirectX delay

	return true;
}

void OdeumEngine::Uninitialize() 
{
	delete m_gameInterface; m_gameInterface = nullptr;
	m_renderObjects.clear();
	delete m_renderer; m_renderer = nullptr;
	delete m_mainCamera; m_mainCamera = nullptr;
}


void OdeumEngine::UpdateCamera() 
{
	DirectX::XMFLOAT3 move;
	move = m_mainCamera->Eye();

	if (Input::GetInstance()->kb.Up || Input::GetInstance()->kb.W)
		move.z -= 0.05f;

	if (Input::GetInstance()->kb.Down || Input::GetInstance()->kb.S)
		move.z += 0.05f;

	if (Input::GetInstance()->kb.Left || Input::GetInstance()->kb.A)
		move.x += 0.05f;

	if (Input::GetInstance()->kb.Right || Input::GetInstance()->kb.D)
		move.x -= 0.05f;

	if (Input::GetInstance()->kb.PageUp || Input::GetInstance()->kb.Space)
		move.y += 0.05f;

	if (Input::GetInstance()->kb.PageDown || Input::GetInstance()->kb.X)
		move.y -= 0.05f;

	Input::GetInstance()->m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	if (Input::GetInstance()->mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(float(Input::GetInstance()->mouse.x) * ROTATION_GAIN, float(Input::GetInstance()->mouse.y) * ROTATION_GAIN, 0.f * ROTATION_GAIN);
		m_pitch -= delta.y;
		m_yaw -= delta.x;

		float limit = DirectX::XM_PI / 2.0f - 0.01f;

		m_pitch = max(-limit, m_pitch);
		m_pitch = min(limit, m_pitch);

		if (m_yaw > DirectX::XM_PI)
		{
			m_yaw -= DirectX::XM_PI;
		}
		else if (m_yaw < -DirectX::XM_PI)
		{
			m_yaw += DirectX::XM_PI;
		}
	}

	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	DirectX::XMFLOAT3 up;
	up.x = 0; up.y = 1; up.z = 0;
	DirectX::XMFLOAT3 look;
	look.x = move.x + x; look.y = move.y + y; look.z = move.z - z ;

	m_mainCamera->SetViewMatrix(move, look, up);
}
