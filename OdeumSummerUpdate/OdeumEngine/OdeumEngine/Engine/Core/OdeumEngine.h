#ifndef ODEUMENGINE_H
#define ODEUMENGINE_H

#include "Window.h"

class OdeumEngine
{
public:

	OdeumEngine();
	~OdeumEngine();

	void Run();
	bool Initialize();
	void Uninitialize();
	void Close();

private:
	Window* _window;
	bool _isRunning;
};

#endif