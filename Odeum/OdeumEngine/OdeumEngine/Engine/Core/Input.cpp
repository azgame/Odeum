#include "Input.h"
#include "OdeumEngine.h"

std::unique_ptr<Input> Input::sm_input = nullptr;
Input::InputState<KeyCode> Input::sm_keyPressed = InputState<KeyCode>();
Input::InputState<MouseCode> Input::sm_mousePressed = InputState<MouseCode>();
Vector2 Input::mousePos = Vector2(kZero);

Input& Input::Get()
{
	if (sm_input == nullptr)
	{
		sm_input = std::unique_ptr<Input>();
	}
	return *sm_input.get();
}

void Input::KeyUpdate(KeyEvent& key)
{
	if (key.GetEventType() == EventType::KeyPressed)
		sm_keyPressed.states[key.GetKeyCode()] = true;
	else
		sm_keyPressed.states[key.GetKeyCode()] = false;
}

bool Input::isKeyPressed(KeyCode key)
{
	return sm_keyPressed.states[key];
}

void Input::MouseUpdate(MouseButtonEvent& mouse)
{
	if (mouse.GetEventType() == EventType::MouseButtonPressed)
		sm_mousePressed.states[mouse.GetMouseButton()] = true;
	else if (mouse.GetEventType() == EventType::MouseButtonReleased)
		sm_mousePressed.states[mouse.GetMouseButton()] = false;
}

bool Input::isButtonClicked(MouseCode mouse)
{
	return sm_mousePressed.states[mouse];
}

float Input::GetMouseX()
{
	POINT point;
	GetCursorPos(&point);
	mousePos = Vector2((float)point.x - OdeumEngine::Get().GetWindow().GetXPos(), (float)point.y - OdeumEngine::Get().GetWindow().GetYPos());
	return mousePos.GetX();
}

float Input::GetMouseY()
{
	POINT point;
	GetCursorPos(&point);
	mousePos = Vector2((float)point.x - OdeumEngine::Get().GetWindow().GetXPos(), (float)point.y - OdeumEngine::Get().GetWindow().GetYPos());
	return mousePos.GetY();
}

// gotta setup callback (in core engine)
// glfwSetKeyCallback(window, KeyCallBack);
//void Input::KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
//{
//	// print which key was pressed
//	std::cout << "Key: " << key_ << std::endl;
//
//	// do key event based on which key
//	if (key_ == GLFW_KEY_SPACE) {
//		switch (action_)
//		{
//		case GLFW_PRESS:
//			std::cout << "Key: " << key_ << " has been pressed." << std::endl;
//			break;
//		case GLFW_REPEAT:
//			std::cout << "Key: " << key_ << " is being held." << std::endl;
//			break;
//		case GLFW_RELEASE:
//			std::cout << "Key: " << key_ << " has been released." << std::endl;
//			break;
//		default:
//			break;
//		}
//	}
//	
//}

// gotta setup callback (in core engine)
// glfwSetCursorPosCallback(window, CursorPositionCallback);
//void Input::CursorPositionCallback(GLFWwindow* window_, double xpos_, double ypos_)
//{
//	// print cursor pos
//	std::cout << "Cursor Pos: (" << xpos_ << ", " << ypos_ << ")" << std::endl;
//
//	// do mouse move event
//}

// gotta setup callback (in core engine)
// glfwSetMouseButtonCallback(window, MouseButtonCallback);
//void Input::MouseButtonCallback(GLFWwindow* window_, int button_, int action_, int mods_)
//{
//	// print which button was pressed
//	std::cout << "Button: " << button_ << std::endl;
//
//	// do key event based on which key
//	if (button_ == GLFW_MOUSE_BUTTON_LEFT) {
//		switch (action_)
//		{
//		case GLFW_PRESS:
//			std::cout << "Button: " << button_ << " has been pressed." << std::endl;
//			break;
//		case GLFW_REPEAT:
//			std::cout << "Button: " << button_ << " is being held." << std::endl;
//			break;
//		case GLFW_RELEASE:
//			std::cout << "Button: " << button_ << " has been released." << std::endl;
//			break;
//		default:
//			break;
//		}
//	}
//}

