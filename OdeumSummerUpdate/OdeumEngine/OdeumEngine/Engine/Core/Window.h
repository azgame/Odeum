#ifndef WINDOW_H
#define WINDOW_H

#include "../../pch.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "../Events/Event.h"
#include <functional>

class Window
{
public:

	void Initialize(uint32_t width, uint32_t height, bool vSync, bool ultraWide);
	void Update();

	void UninitializeWindow();

	void SetEventCallback(const std::function<void(Event&)> callback) { m_data.eventCallback = callback; }

	HWND GetHWND() { return glfwGetWin32Window(m_window); }
	const HWND GetHWND() const { return glfwGetWin32Window(m_window); }

	void* GetNativeWindow() { return m_window; }

	int GetWidth() { return m_data.width; }
	int GetHeight() { return m_data.height; }
	bool isUltraWide() { return m_data.ultraWide; }
	bool isVSync() { return m_data.vSync; }

private:

	GLFWwindow*				m_window;
	bool					isRunning, FULL_SCREEN;
	
	struct WindowData
	{
		std::string title;
		uint32_t width, height;
		bool vSync, ultraWide;

		std::function<void(Event&)> eventCallback;
	};

	WindowData m_data;
};

#endif