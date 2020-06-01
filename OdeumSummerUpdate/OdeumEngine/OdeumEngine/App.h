#ifndef _APP_H_
#define _APP_H_

#include "pch.h"

class App
{
public:
	App();
	App(const App&);
	~App();

	bool Initialize();
	void Run();
	void Uninitialize();

	void SetWindowPosition(int x_, int y_);
	void SetWindowSize(int w_, int h_);
	void ToggleFullscreen();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void InitializeWindow();
	void UninitializeWindow();

	LPCWSTR					m_applicationName;
	HINSTANCE				m_hInstance;
	HWND					m_hwnd;

	LONG HWNDStyle = 0;
	LONG HWNDStyleEx = 0;
	WINDOWPLACEMENT wpc;

	int						xPos, yPos, scrWidth, scrHeight;
	bool					isRunning, FULL_SCREEN;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static App* ApplicationHandle = 0;

#endif 