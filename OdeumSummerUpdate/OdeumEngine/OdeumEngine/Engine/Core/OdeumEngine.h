#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "Window.h"
#include "SystemStack.h"

#include "GameInterface.h"
#include "Scene.h"

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	static OdeumEngine& Get() { return *sm_instance; }
	Window& GetWindow() { return *m_window; }

	uint32_t GetCurrentScene() { return m_currentScene; }

	void AddSystem(CoreSystem* system_);

	void Run();
	bool Initialize();
	void Uninitialize();
	void Close();

private:
	Window* m_window;
	bool m_isRunning;

	uint32_t m_currentScene;

	SystemStack m_systemStack;

	static OdeumEngine* sm_instance;
};

#endif