#ifndef KinimaticMovement_H
#define KinimaticMovement_H


#include "../../Engine/Core/Component.h"

class KinimaticMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override {}
	void OnDetach() {};
	void Update(float deltaTime) override;
};

#endif