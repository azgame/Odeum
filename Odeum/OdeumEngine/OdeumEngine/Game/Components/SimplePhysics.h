#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H

#include "../../Engine/Core/Component.h"

class SimplePhysics : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	// other functions
	void Transform(Vector3 translate);
	void ApplyForce(Vector3 force);
	void SetMass(float mass);
	void SetVelocity(Vector3 velocity);
	Vector3 GetVelocity();
	// not sure which we want to keep in the physics component or the game object (ie. pos, vel, acc)
private:
	Vector3 m_acceleration;
	Vector3 m_velocity;
	Vector3 m_position;
	float m_mass;
};
#endif

