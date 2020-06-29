#ifndef _APP_H_
#define _APP_H_

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


#endif 