#ifndef DynamicMovement_H
#define DynamicMovement_H

#include "Rigidbody.h"
#include <algorithm>
#include <set>

class DynamicMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() {};
	void Update(float deltaTime) override;
	void SetAll(float maxAcceleration_, float maxSpeed_, float targetRadius_, float slowRadius_, float timeToTarget_);
	GameObject* object;
	//target is the gameobject we move towards or away from
	GameObject* target;
	float maxAcceleration;
	float maxSpeed;
	float avoidDistance;
	
	
	 std::set<GameObject*> obstacles;
	Rigidbody* rb;
	//when to stop approaching the target
	float targetRadius;
	//when to slow down when approaching the target i.e. "close enough""
	float slowRadius;
	//how long it takes to slow down
	float timeToTarget;

	//Variables that change movement type

	//causes the object to flee from the target instead
	bool shouldFlee;
	//causes the object to avoid obstacles
	bool shouldAvoid;
	Vector4 GetSteering();

};

#endif