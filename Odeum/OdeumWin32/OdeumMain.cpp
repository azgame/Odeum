#include "OdeumMain.h"



OdeumMain::OdeumMain()
{
	m_renderer = nullptr;
	m_mainCamera = nullptr;
}


OdeumMain::~OdeumMain()
{
}

bool OdeumMain::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{

	// Initialize camera
	m_mainCamera = new Camera();
	if (!m_mainCamera) return false;

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd)) return false;

	// Initialize window size dependent resources CreateWindowSizeDependentResources(screenHeight, screenWidth, camera);
	m_renderer->CreateWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);

	m_input->Initialize(hwnd);

	return true;
}

bool OdeumMain::Run() 
{
	// TODO Ryan: Grab Input - m_input. It might need some more functions based on what the guide recommends. Update Input class as needed

	DirectX::XMFLOAT3 move;

	if (m_input->kb.Up || m_input->kb.W)
		move.y += 1.f;

	if (m_input->kb.Down || m_input->kb.S)
		move.y -= 1.f;

	if (m_input->kb.Left || m_input->kb.A)
		move.x += 1.f;

	if (m_input->kb.Right || m_input->kb.D)
		move.x -= 1.f;

	if (m_input->kb.PageUp || m_input->kb.Space)
		move.z += 1.f;

	if (m_input->kb.PageDown || m_input->kb.X)
		move.z -= 1.f;

	DirectX::XMFLOAT3 up;
	up.x = 0; up.y = 1; up.z = 0;
	DirectX::XMFLOAT3 look;
	look.x = 0; look.y = 0; look.z = 1;

	m_mainCamera->SetViewMatrix(move, look, up);
	
	// TODO Ryan: Update Camera. Just use m_mainCamera->SetViewMatrix(eye, look, up) to update the eye and look XMFLOAT3's in there
	if (!m_renderer->Frame()) return false;
	
	return true;
}

void OdeumMain::Uninitialize() 
{
	
}
