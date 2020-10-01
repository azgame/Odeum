#ifndef KinimaticMovement_H
#define KinimaticMovement_H



#include "SimplePhysics.h"

class KinimaticMovement : public Component
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;
	GameObject* object;
	GameObject* target;
	float maxSpeed;
	Vector4 GetSteering();
	SimplePhysics* physicsComp;
};

#endif