#ifndef SIMPLEPHYSICS_H
#define SIMPLEPHYSICS_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class SimplePhysics : public Component
{
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	// other functions
	void Transform(Vector3 translate);
	void ApplyForce(Vector3 force);

	// Getters
	float GetMass() { return m_mass; }
	Vector3 GetPosition() { return m_position; }
	Vector3 GetVelocity() { return m_velocity; }
	Vector3 GetAcceleration() { return m_acceleration; }

	// Setters
	void SetMass(float mass);
	void SetPosition(Vector3 position);
	void SetVelocity(Vector3 velocity);
	void SetAcceleration(Vector3 acceleration);

	// not sure which we want to keep in the physics component or the game object (ie. pos, vel, acc)
private:
	Vector3 m_acceleration;
	Vector3 m_velocity;
	Vector3 m_position;
	float m_mass;
};
#endif

