#ifndef WINDOW_H
#define WINDOW_H

#include "../pch.h"
#include "../Events/Event.h"

class Window
{
public:

	void SetWindowPosition(int x_, int y_);
	void SetWindowSize(int w_, int h_);
	void ToggleFullscreen();

	void InitializeWindow();
	void UninitializeWindow();

private:

	int						_screenWidth, _screenHeight, _xPos, _yPos;
	bool					isRunning, FULL_SCREEN;

	LPCWSTR					m_applicationName;
	HINSTANCE				m_hInstance;
	HWND					m_hwnd;

	LONG HWNDStyle = 0;
	LONG HWNDStyleEx = 0;
	WINDOWPLACEMENT wpc;
};

#endif