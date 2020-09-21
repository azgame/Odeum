#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "Window.h"
#include "Timer.h"
#include "SystemStack.h"

#include "GameInterface.h"
#include "Scene.h"
#include "../Events/Event.h"
#include "GameObject.h"
#include "../Rendering/DirectX12/SceneGraph.h"
#include "Camera.h"

#include "TestRender.h"

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

class WindowCloseEvent;
class WindowResizeEvent;

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	static OdeumEngine& Get() { return *sm_instance; }
	Window& GetWindow() { return *m_window; }
	Timer& GetTimer() { return m_engineTimer; }
	Camera& GetCamera() { return m_camera; }

	void SetGameInterface(GameInterface* gameInterface_) { m_gameInterface = gameInterface_; }
	uint32_t GetCurrentScene() { return m_currentScene; }
	void SetCurrentScene(int sceneNum_) { m_currentScene = sceneNum_; }

	void AddSystem(CoreSystem* system_);

	void OnEvent(Event& e);
	void Run();
	bool Initialize();
	void Uninitialize();

	bool Close(WindowCloseEvent& closeEvent);
	bool Resize(WindowResizeEvent& resizeEvent);

private:
	Window* m_window;
	Timer m_engineTimer;
	Camera m_camera;

	bool m_isRunning;
	uint32_t m_currentScene;

	GameInterface* m_gameInterface;

	SystemStack m_systemStack;

	TestRender testCase;

	static OdeumEngine* sm_instance;
};

#endif