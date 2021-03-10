#ifndef PLAYEREFFECTSCOMPONENT_H
#define PLAYEREFFECTSCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

#include "../GameObjects/Item.h"
#include "../GameObjects/Observer.h"

class PlayerEffectsComponent : public Component, public Subject 
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) {};

	virtual void CheckForProccedEvents(GameEvent& Event);
};

#endif