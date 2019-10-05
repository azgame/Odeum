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
	/*void KeyDown(unsigned int);
	void KeyUp(unsigned int);*/

	static Input* getInstance();

	void Update(HWND hwnd);

	/*bool IsKeyDown(unsigned int);*/

	DirectX::Keyboard::State kb;
	DirectX::Mouse::State mouse;

private:
	/*bool m_keys[256];*/
	static Input* s_instance;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};

#endif // !_INPUT_H_