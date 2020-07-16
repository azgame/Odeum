#include "pch.h"
#include "OdeumEngine.h"


OdeumEngine::OdeumEngine()
{
}

OdeumEngine::~OdeumEngine()
{
}

void OdeumEngine::Run()
{
	_isRunning = true;

	while (_isRunning)
	{
		_window->Update();
	}
}

bool OdeumEngine::Initialize()
{	
	_window = new Window();

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	_window->InitializeWindow();

	std::function<void()> fcnPtr = std::bind(&OdeumEngine::Close, this);
	_window->SetCloseEvent(fcnPtr);

	return true;
}

void OdeumEngine::Uninitialize()
{
	_window->UninitializeWindow();
	SAFE_DELETE(_window);
}

void OdeumEngine::Close()
{
	_isRunning = false;
}
