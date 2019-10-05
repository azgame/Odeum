#include "Input.h"


Input *Input::s_instance = nullptr;


Input::Input()
{
	
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

void Input::Initialize(HWND hwnd)
{
	/*int i;
	for (i = 0; i < 256; i++) 
		m_keys[i] = false;*/
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(hwnd);
}

//void Input::KeyDown(unsigned int input)
//{
//	m_keys[input] = true;
//}
//
//void Input::KeyUp(unsigned int input)
//{
//	m_keys[input] = false;
//}

Input* Input::getInstance()
{
	if (!s_instance) s_instance = new Input();
	return s_instance;
}

void Input::Update(HWND hwnd) {
	kb = m_keyboard->GetState();
	mouse = m_mouse->GetState();
}

//bool Input::IsKeyDown(unsigned int key)
//{
//	return m_keys[key];
//}
