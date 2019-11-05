#include "OdeumMain.h"



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

	// Initialize objects -- comment out for now so that it works
	/*for (auto object : m_renderObjects) {
		object->Initialize(m_renderer->GetD3DDevice(), m_renderer->GetCommandList());
	}*/

	// Initialize renderer
	m_renderer = new Renderer();
	if (!m_renderer->Initialize(screenHeight, screenWidth, hwnd, m_renderObjects)) return false;
	// if (!m_renderer->InitializeRaytrace(screenHeight, screenWidth, hwnd)) return false;

	// Initialize window size dependent resources CreateWindowSizeDependentResources(screenHeight, screenWidth, camera);
	m_renderer->CreateRasterWindowSizeDependentResources(screenHeight, screenWidth, m_mainCamera);

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
		move.z -= 0.2f;

	if (m_input->kb.Down || m_input->kb.S)
		move.z += 0.2f;

	if (m_input->kb.Left || m_input->kb.A)
		move.x -= 0.2f;

	if (m_input->kb.Right || m_input->kb.D)
		move.x += 0.2f;

	if (m_input->kb.PageUp || m_input->kb.Space)
		move.y += 0.2f;

	if (m_input->kb.PageDown || m_input->kb.X)
		move.y -= 0.2f;

	DirectX::XMFLOAT3 up;
	up.x = 0; up.y = 1; up.z = 0;
	DirectX::XMFLOAT3 look;
	look.x = move.x; look.y = move.y; look.z = move.z - 1.0f;

	m_mainCamera->SetViewMatrix(move, look, up);
}
