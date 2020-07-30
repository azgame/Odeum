#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "Window.h"

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	void Run();
	bool Initialize();
	void Uninitialize();
	void Close();

private:
	Window* m_window;
	bool m_isRunning;
};

#endif