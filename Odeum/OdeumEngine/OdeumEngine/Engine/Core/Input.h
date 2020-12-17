#ifndef INPUT_H
#define INPUT_H

#include "../../pch.h"
#include "Window.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Math/Vector.h"

#include <map>

using namespace Key;
using namespace Mouse;

class Input {
public:

	Input() = delete;
	Input(const Input&) = delete;
	Input(Input&& Input) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

	static Input& Get();
	void Reset() 
	{ 
		sm_keyPressed = InputState<KeyCode>();
		sm_mousePressed = InputState<MouseCode>();
	}

	void KeyUpdate(KeyEvent& key);
	bool isKeyPressed(KeyCode key);
	void MouseUpdate(MouseButtonEvent& mouse);
	bool isButtonClicked(MouseCode mouse);

	float GetMouseX();
	float GetMouseY();
	
	template<typename T>
	struct InputState
	{
		InputState()
		{
			states.clear();
		}

		std::map<T, bool> states;
	};

private:

	static InputState<KeyCode> sm_keyPressed;
	static InputState<MouseCode> sm_mousePressed;

	static Vector2 mousePos;
	static Vector2 lastMousePos;

	static std::unique_ptr<Input> sm_input;
};


// KeyPress
	// gotta setup callback (in core engine)
	// glfwSetKeyCallback(window, KeyCallBack);
	//void KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_);

// Mouse Cursor
	// gotta setup callback (in core engine)
	// glfwSetCursorPosCallback(window, CursorPositionCallback);
	//void CursorPositionCallback(GLFWwindow* window_, double xpos_, double ypos_);

// Mouse Button
	// gotta setup callback (in core engine)
	// glfwSetMouseButtonCallback(window, MouseButtonCallback);
	//void MouseButtonCallback(GLFWwindow* window_, int button_, int action_, int mods_);

#endif