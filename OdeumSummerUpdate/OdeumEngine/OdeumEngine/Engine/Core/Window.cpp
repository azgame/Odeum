#include "Window.h"

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Window* WindowHandle = 0;

void Window::Update()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		// handle destroy
		Close();
		PostQuitMessage(0);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_CLOSE:
		// handle destroy
		Close();
		PostQuitMessage(0);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_KEYDOWN: // Move into input handling
		switch (wparam)
		{
		case VK_SPACE:
			// handle fullscreen
			ToggleFullscreen();
			break;
		default:
			break;
		}
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case SC_MINIMIZE:
		// handle minimized
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case SC_RESTORE:
		// handle restore
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_MOVE:
		SetWindowPosition((int)LOWORD(lparam), (int)HIWORD(lparam));
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_SIZE:
		SetWindowSize((int)LOWORD(lparam), (int)HIWORD(lparam));
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	default:
		return WindowHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}

void Window::InitializeWindow()
{
	WindowHandle = this;

	WNDCLASSEX wc;

	// Get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name
	m_applicationName = L"OdeumEngine";

	// Setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen
	m_screenHeight = GetSystemMetrics(SM_CYSCREEN);
	m_screenWidth = GetSystemMetrics(SM_CXSCREEN);

	// Start full screen or windowed
	FULL_SCREEN = false;

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;

		// If full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)m_screenHeight;
		dmScreenSettings.dmPelsWidth = (unsigned long)m_screenWidth;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		m_xPos = m_yPos = 0;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_POPUP,
			m_xPos, m_yPos, m_screenWidth, m_screenHeight, NULL, NULL, m_hInstance, NULL);
	}
	else
	{
		m_screenWidth = 1600;
		m_screenHeight = 900;

		// Place the window in the middle of the screen
		m_xPos = (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
		m_yPos = (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
			m_xPos, m_yPos, m_screenWidth, m_screenHeight, NULL, NULL, m_hInstance, NULL);
	}

	// Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(true);
}

void Window::UninitializeWindow()
{
	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	WindowHandle = NULL;
}

void Window::SetCloseEvent(std::function<void()> func_)
{
	closeFunc = func_;
}

void Window::Close()
{
	closeFunc();
}

void Window::SetWindowPosition(int x_, int y_)
{
	m_xPos = x_;
	m_yPos = y_;
}

void Window::SetWindowSize(int w_, int h_)
{
	m_screenWidth = w_;
	m_screenHeight = h_;
}

void Window::ToggleFullscreen()
{
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode
	if (!FULL_SCREEN)
	{
		FULL_SCREEN = true;

		GetWindowPlacement(m_hwnd, &wpc);
		if (HWNDStyle == 0)
			HWNDStyle = GetWindowLong(m_hwnd, GWL_STYLE);
		if (HWNDStyleEx == 0)
			HWNDStyleEx = GetWindowLong(m_hwnd, GWL_EXSTYLE);

		LONG newHWNDStyle = HWNDStyle;
		newHWNDStyle &= ~WS_BORDER;
		newHWNDStyle &= ~WS_DLGFRAME;
		newHWNDStyle &= ~WS_THICKFRAME;

		LONG newHWNDStyleEx = HWNDStyleEx;
		newHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

		SetWindowLong(m_hwnd, GWL_STYLE, newHWNDStyle | WS_POPUP);
		SetWindowLong(m_hwnd, GWL_EXSTYLE, newHWNDStyleEx | WS_EX_TOPMOST);
		ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
		SetWindowPosition(0, 0);
		SetWindowSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		FULL_SCREEN = false;
		SetWindowLong(m_hwnd, GWL_STYLE, HWNDStyle);
		SetWindowLong(m_hwnd, GWL_EXSTYLE, HWNDStyleEx);
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		SetWindowPlacement(m_hwnd, &wpc);
		SetWindowPosition(wpc.rcNormalPosition.left, wpc.rcNormalPosition.top);
		SetWindowSize(wpc.rcNormalPosition.right - wpc.rcNormalPosition.left, wpc.rcNormalPosition.bottom - wpc.rcNormalPosition.top);
	}
}