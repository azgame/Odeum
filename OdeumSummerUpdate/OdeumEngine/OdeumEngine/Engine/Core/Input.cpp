#include "Input.h"
#include "OdeumEngine.h"

// gotta setup callback (in core engine)
// glfwSetKeyCallback(window, KeyCallBack);
void Input::KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
{
	// print which key was pressed
	std::cout << "Key: " << key_ << std::endl;

	// do key event based on which key
	if (key_ == GLFW_KEY_SPACE) {
		switch (action_)
		{
		case GLFW_PRESS:
			std::cout << "Key: " << key_ << " has been pressed." << std::endl;
			break;
		case GLFW_REPEAT:
			std::cout << "Key: " << key_ << " is being held." << std::endl;
			break;
		case GLFW_RELEASE:
			std::cout << "Key: " << key_ << " has been released." << std::endl;
			break;
		default:
			break;
		}
	}
	
}

// gotta setup callback (in core engine)
// glfwSetCursorPosCallback(window, CursorPositionCallback);
void Input::CursorPositionCallback(GLFWwindow* window_, double xpos_, double ypos_)
{
	// print cursor pos
	std::cout << "Cursor Pos: (" << xpos_ << ", " << ypos_ << ")" << std::endl;

	// do mouse move event
}

// gotta setup callback (in core engine)
// glfwSetMouseButtonCallback(window, MouseButtonCallback);
void Input::MouseButtonCallback(GLFWwindow* window_, int button_, int action_, int mods_)
{
	// print which button was pressed
	std::cout << "Button: " << button_ << std::endl;

	// do key event based on which key
	if (button_ == GLFW_MOUSE_BUTTON_LEFT) {
		switch (action_)
		{
		case GLFW_PRESS:
			std::cout << "Button: " << button_ << " has been pressed." << std::endl;
			break;
		case GLFW_REPEAT:
			std::cout << "Button: " << button_ << " is being held." << std::endl;
			break;
		case GLFW_RELEASE:
			std::cout << "Button: " << button_ << " has been released." << std::endl;
			break;
		default:
			break;
		}
	}
}

