#include "OdeumMain.h"

const float ROTATION_GAIN = 0.004f;
const float MOVEMENT_GAIN = 0.07f;


OdeumMain::OdeumMain()
{
	m_renderer = nullptr;
	m_mainCamera = nullptr;

	// For now hardcode number of objects
	m_renderObjects.push_back(new Model());
}


OdeumMain::~OdeumMain()
{
}

bool OdeumMain::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{

	// Initialize camera
	m_mainCamera = new Camera();
	if (!m_mainCamera) return false;

	m_pitch = m_yaw = 0;

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd, m_renderObjects)) return false;

	// Initialize window size dependent resources CreateWindowSizeDependentResources(screenHeight, screenWidth, camera);
	m_renderer->CreateRasterWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);
	m_renderer->CreateRaytracingWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);

	return true;
}

bool OdeumMain::Run() 
{
	UpdateCamera();
	
	if (!m_renderer->Render(m_renderObjects)) return false;

	return true;
}

void OdeumMain::Uninitialize() 
{
	
}

// TODO Ryan: Add mouse controls to rotate the camera around
void OdeumMain::UpdateCamera() 
{
	DirectX::XMFLOAT3 move;
	move = m_mainCamera->Eye();

	if (m_input->kb.Up || m_input->kb.W)
		move.z -= 0.05f;

	if (m_input->kb.Down || m_input->kb.S)
		move.z += 0.05f;

	if (m_input->kb.Left || m_input->kb.A)
		move.x += 0.05f;

	if (m_input->kb.Right || m_input->kb.D)
		move.x -= 0.05f;

	if (m_input->kb.PageUp || m_input->kb.Space)
		move.y += 0.05f;

	if (m_input->kb.PageDown || m_input->kb.X)
		move.y -= 0.05f;

	m_input->m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	if (m_input->mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{

		DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(float(m_input->mouse.x) * ROTATION_GAIN, float(m_input->mouse.y) * ROTATION_GAIN, 0.f * ROTATION_GAIN);
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
