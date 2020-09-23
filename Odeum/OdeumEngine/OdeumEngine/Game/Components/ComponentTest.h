#ifndef COMPONENTTEST_H
#define COMPONENTTEST_H

#include "../../Engine/Core/Component.h"

class ComponentTest : public Component
{
public:
	void OnAttach(GameObject* parent) override {}
	void OnDetach() {};
	void Update(float deltaTime) override;
};

#endif