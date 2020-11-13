#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "../../pch.h"

#include "GameInterface.h"
#include "Scene.h"
#include "Window.h"
#include "Timer.h"
#include "Camera.h"
#include "SystemStack.h"
#include "GameObject.h"
#include "Input.h"
#include "../Events/Event.h"

class WindowCloseEvent;
class WindowResizeEvent;
class KeyEvent;

class AbstractRenderer;

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

private:

	bool Close(WindowCloseEvent& closeEvent);
	bool Resize(WindowResizeEvent& resizeEvent);
	bool KeyboardInput(KeyEvent& keyEvent);

	int GetSceneIndex(std::string fileName);
	void LoadEngineProfile(std::string FileName, std::wstring& WindowName, uint32_t& Width, uint32_t& Height, bool& VSync, bool& UltraWide);
	void LoadGameSceneIndex(std::string FileName);

	GameInterface*			m_gameInterface;
	Window*					m_window;
	Timer					m_engineTimer;
	Camera					m_camera;
	SystemStack				m_systemStack;

	AbstractRenderer*		m_renderer;

	bool					m_isRunning;
	uint32_t				m_currentScene;

	static OdeumEngine*		sm_instance;
};

#endif