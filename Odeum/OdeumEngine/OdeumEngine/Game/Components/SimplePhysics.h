#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class SimplePhysics : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	// other functions
	void Transform(Vector4 translate);
	void ApplyForce(Vector4 force);

	// Getters
	float GetMass();
	Vector4 GetPosition();
	Vector4 GetVelocity();
	//Vector4 GetAcceleration();

	// Setters
	void SetMass(float mass);

	void SetVelocity(Vector3 velocity);
	//Vector3 GetVelocity();
	// not sure which we want to keep in the physics component or the game object (ie. pos, vel, acc)
	void SetAcceleration(Vector4 acceleration );
	void SetPosition(Vector4 position);
	void SetVelocity(Vector4 velocity);
	


private:
	// options for now
	Vector4 totalForce;
	Vector4 totalAcceleration;

};
#endif

