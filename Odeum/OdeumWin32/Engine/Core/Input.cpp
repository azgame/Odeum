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
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(hwnd);
}


Input* Input::getInstance()
{
	if (!s_instance) s_instance = new Input();
	return s_instance;
}

void Input::Update() {
	kb = m_keyboard->GetState();
	mouse = m_mouse->GetState();
}
