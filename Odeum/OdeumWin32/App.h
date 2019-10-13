#ifndef _APP_H_
#define _APP_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "OdeumMain.h"

#include "Input.h"
#include "Renderer.h"

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
	void InitializeWindow(int&, int&);
	void UninitializeWindow();

	LPCWSTR						m_applicationName;
	HINSTANCE					m_hInstance;
	HWND						m_hwnd;
	Input*						m_Input = Input::getInstance();
	OdeumMain*					m_engine;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static App* ApplicationHandle = 0;

#endif // !_APP_H_