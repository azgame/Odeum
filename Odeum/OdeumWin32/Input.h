#ifndef _INPUT_H_
#define _INPUT_H_

#include "pch.h"


class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Initialize(HWND hwnd);
	

	static Input* getInstance();

	void Update();

	

	DirectX::Keyboard::State kb;
	DirectX::Mouse::State mouse;

private:
	static Input* s_instance;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};

#endif // !_INPUT_H_