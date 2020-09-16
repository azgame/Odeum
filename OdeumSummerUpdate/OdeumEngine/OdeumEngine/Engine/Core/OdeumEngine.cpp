#include "OdeumEngine.h"

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Events/ApplicationEvent.h"

OdeumEngine* OdeumEngine::sm_instance = nullptr;

OdeumEngine::OdeumEngine()
{
	sm_instance = this;

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);
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
	for (auto system : m_systemStack)
	{
		if (e.handled)
			break;
		system->HandleEvent(e);
	}
}

void OdeumEngine::Run()
{
	m_isRunning = true;

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
	m_window = new Window();
	
	m_engineTimer.Initialize();

	m_window->InitializeWindow();

	std::function<void()> fcnPtr = std::bind(&OdeumEngine::Close, this);
	m_window->SetCloseEvent(fcnPtr);

	DXGraphics::Initialize();

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

void OdeumEngine::Close()
{
	m_isRunning = false;
}
