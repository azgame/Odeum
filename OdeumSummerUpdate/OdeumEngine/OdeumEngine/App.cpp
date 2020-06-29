#include "pch.h"

#include "App.h"
#include <crtdbg.h>


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static App* ApplicationHandle = 0;


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
	ApplicationHandle = this;

	_window = new Window();

	Debug::DebugInit();
	Debug::SetSeverity(MessageType::TYPE_INFO);

	_window->InitializeWindow(WndProc);

	return true;
}

void App::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	_isRunning = true;
	while (_isRunning)
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
	_window->UninitializeWindow();
	SAFE_DELETE(_window);
	ApplicationHandle = NULL;
}

LRESULT App::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		// handle destroy
		PostQuitMessage(0);
		_isRunning = false;
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_CLOSE:
		// handle destroy
		PostQuitMessage(0);
		_isRunning = false;
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_SPACE:
			// handle fullscreen
			_window->ToggleFullscreen();
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
		_window->SetWindowPosition((int)LOWORD(lparam), (int)HIWORD(lparam));
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	case WM_SIZE:
		_window->SetWindowSize((int)LOWORD(lparam), (int)HIWORD(lparam));
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
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}