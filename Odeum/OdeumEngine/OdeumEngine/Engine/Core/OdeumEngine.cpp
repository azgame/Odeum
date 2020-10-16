#include "OdeumEngine.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"

#include "TestRender.h"

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

		m_camera.UpdateCamera();

		m_gameInterface->Update(timeStep); // update game scene

		for (auto system : m_systemStack)
			system->Update(timeStep);

		m_gameInterface->UIRender(); // render game scene ui

		for (auto system : m_systemStack)
			system->UIRender(); // render system specific ui

		DXGraphics::Present(); // present cumulative rendering to screen
	}
}

bool OdeumEngine::Initialize()
{	
	m_window->SetEventCallback(BIND_EVENT_FN(OdeumEngine::OnEvent));
	m_window->Initialize(1600, 900, false, false);
	
	m_engineTimer.Initialize();

	DXGraphics::Initialize();

	if (!m_gameInterface->Initialize())
		Debug::Error("Could not initialize game scene", __FILENAME__, __LINE__);

	m_currentScene = GetSceneIndex("SceneLoad.txt");
	m_gameInterface->Update(m_engineTimer.GetDeltaTime());

	// Test
	m_systemStack.Push(new TestRender());

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
