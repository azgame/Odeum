#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "Window.h"

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	static OdeumEngine& Get() { return *m_instance; }
	Window& GetWindow() { return *m_window; }

	void Run();
	bool Initialize();
	void Uninitialize();
	void Close();

private:
	Window* m_window;
	bool m_isRunning;

	static OdeumEngine* m_instance;
};

#endif