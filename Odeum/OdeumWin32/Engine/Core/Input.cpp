#include "Input.h"


std::unique_ptr<Input> Input::s_Instance = nullptr;


Input::Input()
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


Input* Input::GetInstance()
{
	if (s_Instance.get() == nullptr) {
		s_Instance.reset(new Input);
	}
	return s_Instance.get();
}

void Input::Update() {
	kb = m_keyboard->GetState();
	mouse = m_mouse->GetState();
}
