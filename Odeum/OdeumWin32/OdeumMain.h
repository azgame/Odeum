#ifndef _ODEUMMAIN_H
#define _ODEUMMAIN_H

#include "pch.h"

#include "Input.h"
#include "Renderer.h"
#include "Camera.h"

class OdeumMain
{
public:
	OdeumMain();
	~OdeumMain();

	bool Initialize(int, int, HWND);
	bool Run();
	void Uninitialize();

private:
	Input* m_input = Input::getInstance();
	Renderer* m_renderer;
	Camera* m_mainCamera;
};

#endif // !_ODEUMMAIN_H