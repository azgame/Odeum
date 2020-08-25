#ifndef INPUT_H
#define INPUT_H

#include "../../pch.h"

class Input {
public:
	// Get the 'singleton' of Input, Also im unsure if it needs to be a singleton? could just be a static class
	// Also could just all go into an eventlistener
	static Input& getInstance() {
		static Input instance;
		return instance;
	}
	Input(Input const&) = delete;
	void operator=(Input const&) = delete;

	static bool IsKeyPressed();
	static bool IsKeyReleased();
private:
	Input(){}
	// Unsure if this part is needed as im deleting the functions in public
	//Input(Input const&);
	//void operator=(Input const&);
};

#endif // !INPUT_H

