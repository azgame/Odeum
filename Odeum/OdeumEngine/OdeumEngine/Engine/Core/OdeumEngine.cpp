#include "OdeumEngine.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/D3DRenderer.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"

#include "TestRender.h"

#include "../JSONUtility/json.hpp"
#include "Utility.h"

OdeumEngine* OdeumEngine::sm_instance = nullptr;

OdeumEngine::OdeumEngine()
{
	sm_instance = this;

	m_currentScene = 0;
	m_isRunning = true;

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	m_window = new Window();
	m_camera = Camera();
	m_renderer = new D3DRenderer();
}

OdeumEngine::~OdeumEngine()
{
	Uninitialize();
}

void OdeumEngine::AddSystem(CoreSystem* system_)
{
	m_systemStack.Push(system_);
	system_->Attach();
}

void OdeumEngine::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OdeumEngine::Close));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OdeumEngine::Resize));

	for (auto system : m_systemStack)
	{
		if (e.handled)
			break;
		system->HandleEvent(e);
	}
}

void OdeumEngine::Run()
{
	while (m_isRunning)
	{
		m_engineTimer.UpdateFrameTicks();
		float timeStep = m_engineTimer.GetDeltaTime();

		m_window->Update(); // Process windows events

		m_gameInterface->Update(timeStep); // update game scene

		m_camera.UpdateCamera();

		m_renderer->Update(timeStep);

		for (auto system : m_systemStack)
			system->Update(timeStep);

		m_renderer->Render(m_camera, timeStep);

		m_gameInterface->UIRender(); // render game scene ui

		for (auto system : m_systemStack)
			system->UIRender(); // render system specific ui

		m_renderer->UIRender();

		DXGraphics::Present(); // present cumulative rendering to screen
	}
}

bool OdeumEngine::Initialize()
{	
	std::wstring windowName;
	uint32_t windowWidth, windowHeight;
	bool vSync, ultraWide;

	LoadEngineProfile("Engine/Config/EngineConfig.json", windowName, windowWidth, windowHeight, vSync, ultraWide);

	m_window->SetEventCallback(BIND_EVENT_FN(OdeumEngine::OnEvent));
	m_window->Initialize(windowName, windowWidth, windowHeight, vSync, ultraWide);

	m_camera.SetAspectRatio((float)windowWidth / (float)windowHeight);
	
	m_engineTimer.Initialize();

	DXGraphics::Initialize();

	if (!m_gameInterface->Initialize())
	{
		Debug::Error("Could not initialize game scene", __FILENAME__, __LINE__);
		return false;
	}

	// m_currentScene = GetSceneIndex("SceneLoad.txt");
	LoadGameSceneIndex("Engine/Config/EngineConfig.json");
	m_gameInterface->Update(m_engineTimer.GetDeltaTime());

	// Test
	// m_systemStack.Push(new TestRender());

	m_renderer->Initialize(*m_window);

	return true;
}

void OdeumEngine::Uninitialize()
{
	m_window->UninitializeWindow();
	SAFE_DELETE(m_window);

 	DXGraphics::Shutdown();
}

bool OdeumEngine::Close(WindowCloseEvent& closeEvent)
{
	m_isRunning = false;
	return true;
}

bool OdeumEngine::Resize(WindowResizeEvent& resizeEvent)
{
	DXGraphics::Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
	return true;
}

// temp way to load game scenes
int OdeumEngine::GetSceneIndex(std::string fileName)
{
	int sceneNum = 0;
	std::string line;
	std::ifstream sceneFile(fileName);
	if (sceneFile.is_open())
	{
		while (std::getline(sceneFile, line))
		{
			sceneNum = std::stoi(line);
		}
	}

	sceneFile.close();
	return sceneNum;
}

void OdeumEngine::LoadEngineProfile(std::string FileName, std::wstring& WindowName, uint32_t& Width, uint32_t& Height, bool& VSync, bool& UltraWide)
{
	std::ifstream configFile(FileName);
	nlohmann::json jsonReader;

	configFile >> jsonReader;
	configFile.close();

	WindowName = Utility::MakeWStr(jsonReader["Config"]["GameName"].get<std::string>());
	Width =jsonReader["Config"]["WindowWidth"].get<int>();
	Height = jsonReader["Config"]["WindowHeight"].get<int>();
	VSync = jsonReader["Config"]["VSync"].get<bool>();
	UltraWide = jsonReader["Config"]["UltraWide"].get<bool>();
}

void OdeumEngine::LoadGameSceneIndex(std::string FileName)
{
	std::ifstream configFile(FileName);
	nlohmann::json jsonReader;

	configFile >> jsonReader;
	configFile.close();

	m_currentScene = jsonReader["Config"]["Scene"].get<int>();
}
