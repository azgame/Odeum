#ifndef KINEMATICMOVEMENT_H
#define KINEMATICMOVEMENT_H

#include "Rigidbody.h"

class KinematicMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	GameObject* object;
	//target is what we are moving towards
	GameObject* target;
	float maxSpeed;
	Vector4 GetSteering();
	//will make the character run away from target instead
	bool shouldFlee;
	
};

#endif