#ifndef DynamicMovement_H
#define DynamicMovement_H



#include "Rigidbody.h"

class DynamicMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	void SetAll(float maxAcceleration_, float maxSpeed_, float targetRadius_, float slowRadius_, float timeToTarget_);
	GameObject* object;
	//target is the gameobject we move towards or away from
	GameObject* target;
	float maxAcceleration;
	float maxSpeed;
	float avoidDistance;
	float lookAhead;
	//when to stop approaching the target
	float targetRadius;
	//when to slow down when approaching the target i.e. "close enough""
	float slowRadius;
	//how long it takes to slow down
	float timeToTarget;
	//causes the object to flee from the target instead
	bool shouldFlee;
	Vector4 GetSteering();

};

#endif