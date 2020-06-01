#include "App.h"
#include <crtdbg.h>

// WINAPI entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	App* app;
	app = new App();
	assert(app);

	if (app->Initialize())
		app->Run();

	app->Uninitialize();
	SAFE_DELETE(app);

	_CrtDumpMemoryLeaks();

	return 0;
}

App::App() {}

App::App(const App& other) {}

App::~App() {}


bool App::Initialize()
{
	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	scrWidth = scrHeight = 0;
	xPos = yPos = 0;

	InitializeWindow();

	return true;
}

void App::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	isRunning = true;
	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void App::Uninitialize()
{
	UninitializeWindow();
}

LRESULT App::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		// handle destroy
		PostQuitMessage(0);
		isRunning = false;
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_CLOSE:
		// handle destroy
		PostQuitMessage(0);
		isRunning = false;
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_KEYDOWN:
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

void App::InitializeWindow()
{
	WNDCLASSEX wc;
	

	// Get an external pointer to this object	
	ApplicationHandle = this;

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
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	scrWidth = GetSystemMetrics(SM_CXSCREEN);

	// Start full screen or windowed
	FULL_SCREEN = false;

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;

		// If full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)scrHeight;
		dmScreenSettings.dmPelsWidth = (unsigned long)scrWidth;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		xPos = yPos = 0;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_POPUP,
			xPos, yPos, scrWidth, scrHeight, NULL, NULL, m_hInstance, NULL);
	}
	else
	{
		// If windowed then set it to 1280x720 resolution
		scrWidth = 1600;
		scrHeight = 900;

		// Place the window in the middle of the screen
		xPos = (GetSystemMetrics(SM_CXSCREEN) - scrWidth) / 2;
		yPos = (GetSystemMetrics(SM_CYSCREEN) - scrHeight) / 2;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
			xPos, yPos, scrWidth, scrHeight, NULL, NULL, m_hInstance, NULL);
	}

	// Bring the window up on the screen and set it as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(true);
}

void App::UninitializeWindow()
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

	// Release the pointer to this class
	ApplicationHandle = NULL;
}

void App::SetWindowPosition(int x_, int y_)
{
	xPos = x_;
	yPos = y_;
	Debug::Info("Window pos: " + std::to_string(xPos) + ", " + std::to_string(yPos), __FILENAME__, __LINE__);
}

void App::SetWindowSize(int w_, int h_)
{
	scrWidth = w_;
	scrHeight = h_;
	Debug::Info("Window size: " + std::to_string(scrWidth) + ", " + std::to_string(scrHeight), __FILENAME__, __LINE__);
}

void App::ToggleFullscreen()
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

		LONG NewHWNDStyle = HWNDStyle;
		NewHWNDStyle &= ~WS_BORDER;
		NewHWNDStyle &= ~WS_DLGFRAME;
		NewHWNDStyle &= ~WS_THICKFRAME;

		LONG NewHWNDStyleEx = HWNDStyleEx;
		NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

		SetWindowLong(m_hwnd, GWL_STYLE, NewHWNDStyle | WS_POPUP);
		SetWindowLong(m_hwnd, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}