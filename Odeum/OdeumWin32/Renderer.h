#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
#include "DeviceResources.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&);
	~Renderer();

	bool Initialize(int, int, HWND);
	void Uninitialize();
	bool Frame();

private:
	bool Render();

	DeviceResources*			m_deviceResources;
};

#endif // !_RENDERER_H_