#include "Window.h"

#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

// Win32 Window setup

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
	{
		WindowCloseEvent close = WindowCloseEvent();
		m_data.eventCallback(close);
		PostQuitMessage(0);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	case WM_CLOSE:
	{
		WindowCloseEvent close = WindowCloseEvent();
		m_data.eventCallback(close);
		PostQuitMessage(0);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	case SC_MINIMIZE:
		// handle minimized
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case SC_RESTORE:
		// handle restore
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_MOVE:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_SIZE:
	{
		WindowResizeEvent resize = WindowResizeEvent((int)LOWORD(lparam), (int)HIWORD(lparam));
		m_data.width = (int)LOWORD(lparam);
		m_data.height = (int)HIWORD(lparam);
		m_data.eventCallback(resize);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	case WM_KEYDOWN:
	{
		KeyPressedEvent key = KeyPressedEvent(static_cast<KeyCode>(wparam), 1); // change 1 later
		m_data.eventCallback(key);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	case WM_KEYUP:
	{
		KeyReleasedEvent key = KeyReleasedEvent(static_cast<KeyCode>(wparam));
		m_data.eventCallback(key);
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
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

void Window::Initialize(uint32_t width, uint32_t height, bool vSync, bool ultraWide)
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
	m_data.height = GetSystemMetrics(SM_CYSCREEN);
	m_data.width = GetSystemMetrics(SM_CXSCREEN);

	int m_xPos = 0;
	int m_yPos = 0;

	// Start full screen or windowed
	FULL_SCREEN = false;

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;

		// If full screen set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)m_data.height;
		dmScreenSettings.dmPelsWidth = (unsigned long)m_data.width;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		m_xPos = m_yPos = 0;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_POPUP,
			m_xPos, m_yPos, m_data.width, m_data.height, NULL, NULL, m_hInstance, NULL);
	}
	else
	{
		m_data.width = 1600;
		m_data.height = 900;
		m_data.ultraWide = (m_data.width / m_data.height == 21.0f / 9.0f);

		// Place the window in the middle of the screen
		m_xPos = (GetSystemMetrics(SM_CXSCREEN) - m_data.width) / 2;
		m_yPos = (GetSystemMetrics(SM_CYSCREEN) - m_data.height) / 2;

		// Create the window with the screen settings and get the handle to it
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_OVERLAPPEDWINDOW,
			m_xPos, m_yPos, m_data.width, m_data.height, NULL, NULL, m_hInstance, NULL);
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

// GLFW Window setup

//void Window::Initialize(uint32_t width, uint32_t height, bool vSync, bool ultraWide)
//{
//	m_data.width = width;
//	m_data.height = height;
//	m_data.vSync = vSync;
//	m_data.ultraWide = ultraWide;
//
//	ASSERT(glfwInit(), "Could not initialize GLFW");
//
//	glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
//	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
//
//	m_window = glfwCreateWindow(width, height, "Odeum Engine", nullptr, nullptr);
//
//	glfwSetWindowUserPointer(m_window, &m_data);
//
//	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//		data.width = width;
//		data.height = height;
//
//		WindowResizeEvent resizeEvent(width, height);
//		data.eventCallback(resizeEvent);
//	});
//
//	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//		WindowCloseEvent event;
//		data.eventCallback(event);
//	});
//
//	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//
//		switch (action)
//		{
//			case GLFW_PRESS:
//			{
//				KeyPressedEvent event((KeyCode)key, 0);
//				data.eventCallback(event);
//				break;
//			}
//			case GLFW_RELEASE:
//			{
//				KeyReleasedEvent event((KeyCode)key);
//				data.eventCallback(event);
//				break;
//			}
//			case GLFW_REPEAT:
//			{
//				KeyPressedEvent event((KeyCode)key, 1);
//				data.eventCallback(event);
//				break;
//			}
//		}
//	});
//
//	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//
//		KeyTypedEvent event((KeyCode)keycode);
//		data.eventCallback(event);
//	});
//
//	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//
//		switch (action)
//		{
//			case GLFW_PRESS:
//			{
//				MouseButtonPressedEvent event((MouseCode)button);
//				data.eventCallback(event);
//				break;
//			}
//			case GLFW_RELEASE:
//			{
//				MouseButtonReleasedEvent event((MouseCode)button);
//				data.eventCallback(event);
//				break;
//			}
//		}
//	});
//
//	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//
//		MouseScrolledEvent event((float)xOffset, (float)yOffset);
//		data.eventCallback(event);
//	});
//
//	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
//	{
//		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
//
//		MouseMovedEvent event((float)xPos, (float)yPos);
//		data.eventCallback(event);
//	});
//}
//
//void Window::Update()
//{
//	glfwPollEvents();
//}
//
//void Window::UninitializeWindow()
//{
//	glfwDestroyWindow(m_window);
//	glfwTerminate();
//}

//void Window::ToggleFullscreen()
//{
//	// Setup the screen settings depending on whether it is running in full screen or in windowed mode
//	if (!FULL_SCREEN)
//	{
//		FULL_SCREEN = true;
//
//		GetWindowPlacement(m_hwnd, &wpc);
//		if (HWNDStyle == 0)
//			HWNDStyle = GetWindowLong(m_hwnd, GWL_STYLE);
//		if (HWNDStyleEx == 0)
//			HWNDStyleEx = GetWindowLong(m_hwnd, GWL_EXSTYLE);
//
//		LONG newHWNDStyle = HWNDStyle;
//		newHWNDStyle &= ~WS_BORDER;
//		newHWNDStyle &= ~WS_DLGFRAME;
//		newHWNDStyle &= ~WS_THICKFRAME;
//
//		LONG newHWNDStyleEx = HWNDStyleEx;
//		newHWNDStyleEx &= ~WS_EX_WINDOWEDGE;
//
//		SetWindowLong(m_hwnd, GWL_STYLE, newHWNDStyle | WS_POPUP);
//		SetWindowLong(m_hwnd, GWL_EXSTYLE, newHWNDStyleEx | WS_EX_TOPMOST);
//		ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
//		SetWindowPosition(0, 0);
//		SetWindowSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
//	}
//	else
//	{
//		FULL_SCREEN = false;
//		SetWindowLong(m_hwnd, GWL_STYLE, HWNDStyle);
//		SetWindowLong(m_hwnd, GWL_EXSTYLE, HWNDStyleEx);
//		ShowWindow(m_hwnd, SW_SHOWNORMAL);
//		SetWindowPlacement(m_hwnd, &wpc);
//		SetWindowPosition(wpc.rcNormalPosition.left, wpc.rcNormalPosition.top);
//		SetWindowSize(wpc.rcNormalPosition.right - wpc.rcNormalPosition.left, wpc.rcNormalPosition.bottom - wpc.rcNormalPosition.top);
//	}
//}