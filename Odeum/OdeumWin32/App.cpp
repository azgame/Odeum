#include "App.h"




App::App()
{
	m_Input = nullptr;
	m_Renderer = nullptr;
}

App::App(const App& other)
{
}


App::~App()
{
}

bool App::Initialize()
{
	int screenHeight, screenWidth;
	screenHeight = screenWidth = 0;

	InitializeWindow(screenHeight, screenWidth);
	
	m_Input->Initialize();

	m_Renderer = new Renderer();
	if (!m_Renderer) return false;

	if (!m_Renderer->Initialize(
		screenHeight, 
		screenWidth, 
		m_hwnd
		))
		return false;
	
	return true;
}

void App::Run()
{
	MSG msg;
	bool isRunning;

	ZeroMemory(&msg, sizeof(MSG));

	isRunning = true;
	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			isRunning = false;
		else
		{
			if (!Frame())
				isRunning = false;
		}	
	}
}

void App::Uninitialize()
{
	if (m_Renderer)
	{
		m_Renderer->Uninitialize();
		delete m_Renderer;
		m_Renderer = nullptr;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	UninitializeWindow();
}

LRESULT App::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		case WM_KEYUP:
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

bool App::Frame()
{
	if (m_Input->IsKeyDown(VK_ESCAPE)) return false;
	if (!m_Renderer->Frame()) return false;

	return true;
}

void App::InitializeWindow(int& screenHeight, int& screenWidth)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
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

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

void App::UninitializeWindow()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		default:
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
