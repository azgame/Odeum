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

void Input::Initialize()
{
	int i;
	for (i = 0; i < 256; i++) 
		m_keys[i] = false;
}

void Input::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

Input* Input::getInstance()
{
	if (!s_instance) s_instance = new Input();
	return s_instance;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
