#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "Window.h"
#include "Timer.h"
#include "SystemStack.h"

#include "GameInterface.h"
#include "Scene.h"

#include "TestRender.h"

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	static OdeumEngine& Get() { return *sm_instance; }
	Window& GetWindow() { return *m_window; }
	Timer& GetTimer() { return m_engineTimer; }

	uint32_t GetCurrentScene() { return m_currentScene; }

	void AddSystem(CoreSystem* system_);

	void Run();
	bool Initialize();
	void Uninitialize();
	void Close();

private:
	Window* m_window;
	Timer m_engineTimer;
	bool m_isRunning;

	uint32_t m_currentScene;

	SystemStack m_systemStack;

	TestRender testCase;

	static OdeumEngine* sm_instance;
};

#endif