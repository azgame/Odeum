#ifndef WINDOW_H
#define WINDOW_H

#include "../Events/Event.h"
#include <functional>

class Window
{
public:

	void Update();
	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	void SetWindowPosition(int x_, int y_);
	void SetWindowSize(int w_, int h_);
	void ToggleFullscreen();

	void InitializeWindow();
	void UninitializeWindow();

	void SetCloseEvent(std::function<void()> func_);
	void Close();

private:

	int						_screenWidth, _screenHeight, _xPos, _yPos;
	bool					isRunning, FULL_SCREEN;
	std::function<void()>	closeFunc;

	LPCWSTR					m_applicationName;
	HINSTANCE				m_hInstance;
	HWND					m_hwnd;

	LONG HWNDStyle = 0;
	LONG HWNDStyleEx = 0;
	WINDOWPLACEMENT wpc;
};

#endif