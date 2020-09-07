#include "OdeumEngine.h"

OdeumEngine* OdeumEngine::m_instance = nullptr;

OdeumEngine::OdeumEngine()
{
	m_instance = this;
}

OdeumEngine::~OdeumEngine()
{
}

void OdeumEngine::Run()
{
	m_isRunning = true;

	while (m_isRunning)
	{
		m_window->Update();
	}
}

bool OdeumEngine::Initialize()
{	
	m_window = new Window();

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	m_window->InitializeWindow();

	std::function<void()> fcnPtr = std::bind(&OdeumEngine::Close, this);
	m_window->SetCloseEvent(fcnPtr);

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
