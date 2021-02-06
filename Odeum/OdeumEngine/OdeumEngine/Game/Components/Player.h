#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class Player : public Component {
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
private:
};

#endif