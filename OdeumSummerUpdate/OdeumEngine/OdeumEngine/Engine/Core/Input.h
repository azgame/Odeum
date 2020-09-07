#ifndef INPUT_H
#define INPUT_H

#include "../../pch.h"

class Input {

public:
	
	static bool IsKeyPressed();
	static bool IsMouseButtonPressed();
	static float GetMouseX();
	static float GetMouseY();
};

#endif

