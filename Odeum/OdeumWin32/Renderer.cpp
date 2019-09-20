#include "Renderer.h"



Renderer::Renderer()
{
	m_deviceResources = nullptr;
}

Renderer::Renderer(const Renderer& other)
{
}


Renderer::~Renderer()
{
}

bool Renderer::Initialize(int screenHeight, int screenWidth, HWND hwnd)
{
	m_deviceResources = new DeviceResources();
	if (!m_deviceResources) return false;
	if (!m_deviceResources->Initialize(screenHeight, screenWidth,  hwnd, VSYNC_ENABLED, FULL_SCREEN))
	{
		MessageBox(hwnd, L"Could not initialize DirectX", L"Error", MB_OK);
		return false;
	}
	
	return true;
}

void Renderer::Uninitialize()
{
	if (m_deviceResources)
	{
		m_deviceResources->Uninitialize();
		delete m_deviceResources;
		m_deviceResources = nullptr;
	}
}

bool Renderer::Frame()
{
	if (!Render()) return false;
	
	return true;
}

bool Renderer::Render()
{
	if (!m_deviceResources->Render()) return false;
	
	return true;
}
