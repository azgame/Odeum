#ifndef KinimaticMovement_H
#define KinimaticMovement_H


#include "../../Engine/Core/Component.h"


class KinimaticMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	//target is what we are moving towards
	GameObject* target;
	float maxSpeed;
	bool shouldFlee;
	//will make the character run away from target instead
private:
	GameObject* object;
	Vector4 GetSteering();

};

#endif