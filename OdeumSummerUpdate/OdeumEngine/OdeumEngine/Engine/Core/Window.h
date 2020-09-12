#ifndef WINDOW_H
#define WINDOW_H

#include "../../pch.h"

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

	HWND GetHWND() { return m_hwnd; }
	const HWND GetHWND() const { return m_hwnd; }

	int GetWidth() { return m_screenWidth; }
	int GetHeight() { return m_screenHeight; }
	int GetXPos() { return m_xPos; }
	int GetYPos() { return m_yPos; }
	bool GetUltraWide() { return m_ultraWideEnabled; }

private:

	int						m_screenWidth, m_screenHeight, m_xPos, m_yPos;
	bool					m_ultraWideEnabled;
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