#ifndef INPUT_H
#define INPUT_H

#include "../../pch.h"
#include "Window.h"
#include "KeyCodes.h"
#include "../Events/KeyEvent.h"

#include <map>

using namespace Key;

class Input {
public:

	Input() = delete;
	Input(const Input&) = delete;
	Input(Input&& Input) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

	static Input& Get();
	void Reset() { sm_keyPressed = InputState(); }

	bool Update(KeyEvent& key);
	bool isKeyPressed(KeyCode key);
	
	struct InputState
	{
		InputState()
		{
			keyStates.clear();
		}

		std::map<KeyCode, bool> keyStates; // TODO Aidan: too large, change this later
	};

	static InputState sm_keyPressed;

private:
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