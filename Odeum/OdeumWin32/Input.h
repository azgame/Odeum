#ifndef _INPUT_H_
#define _INPUT_H_

#include "pch.h"

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Initialize();
	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	// ensures singleton implementation
	static Input* getInstance();

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
	static Input* s_instance;
};

#endif // !_INPUT_H_