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

	return true;
}

bool OdeumMain::Run() 
{
	// TODO Ryan: Grab Input - m_input. It might need some more functions based on what the guide recommends. Update Input class as needed
	// TODO Ryan: Update Camera. Just use m_mainCamera->SetViewMatrix(eye, look, up) to update the eye and look XMFLOAT3's in there
	if (!m_renderer->Frame()) return false;
	
	return true;
}

void OdeumMain::Uninitialize() 
{

}
