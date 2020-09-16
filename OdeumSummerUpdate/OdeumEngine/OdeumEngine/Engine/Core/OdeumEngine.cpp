#include "OdeumEngine.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Events/ApplicationEvent.h"

OdeumEngine* OdeumEngine::sm_instance = nullptr;

OdeumEngine::OdeumEngine()
{
	sm_instance = this;

	m_currentScene = 0;
	m_isRunning = true;

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	m_window = new Window();
}

OdeumEngine::~OdeumEngine()
{
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

		m_window->Update();

		for (auto system : m_systemStack)
			system->Update(timeStep);

		DXGraphics::Present();
	}
}

bool OdeumEngine::Initialize()
{	
	m_window->Initialize(1600, 900, false, false);
	m_window->SetEventCallback(BIND_EVENT_FN(OdeumEngine::OnEvent));
	
	m_engineTimer.Initialize();

	DXGraphics::Initialize();

	// Test
	CoreSystem* testRender = new TestRender();
	m_systemStack.Push(testRender);
	testRender->Attach();

	return true;
}

void OdeumEngine::Uninitialize()
{
	m_window->UninitializeWindow();
	SAFE_DELETE(m_window);
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
