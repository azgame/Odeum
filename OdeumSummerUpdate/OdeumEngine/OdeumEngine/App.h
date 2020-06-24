#ifndef _APP_H_
#define _APP_H_

#include "pch.h"
#include "../Engine/Core/Window.h"

class App
{
public:
	App();
	App(const App&);
	~App();

	bool Initialize();
	void Run();
	void Uninitialize();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	Window* _window;

	bool _isRunning;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static App* ApplicationHandle = 0;


#endif 