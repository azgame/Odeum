#ifndef _APP_H_
#define _APP_H_

#include "pch.h"
#include "Engine/Core/OdeumEngine.h"
#include "Engine/Core/Input.h"

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
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static App* ApplicationHandle = 0;

#endif // !_APP_H_