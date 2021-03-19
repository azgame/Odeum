#ifndef PLAYERMAPCOMPONENT_H
#define PLAYERMAPCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class PlayerMapComponent : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) {};

	void AddMovement(int Movement);

private:
	UINT initiative;

	

};

#endif