#ifndef _INPUT_H_
#define _INPUT_H_

#include "../../pch.h"


class Input
{
public:
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

	Input();
	~Input();

	void Initialize(HWND hwnd);
	

	static Input* GetInstance();

	void Update();

	

	DirectX::Keyboard::State kb;
	DirectX::Mouse::State mouse;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

private:

	static std::unique_ptr<Input> s_Instance;
	friend std::default_delete<Input>;
};

#endif // !_INPUT_H_