#ifndef DynamicMovement_H
#define DynamicMovement_H



#include "SimplePhysics.h"

class DynamicMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	GameObject* object;
	GameObject* target;
	float maxAcceleration;
	float maxSpeed;
	float targetRadius;
	float slowRadius;
	float timeToTarget;
	
	Vector4 GetSteering();
	SimplePhysics* physicsComp;
};

#endif