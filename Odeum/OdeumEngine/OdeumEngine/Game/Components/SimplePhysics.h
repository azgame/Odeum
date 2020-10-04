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
	void ApplyForce(Vector4 force, Vector4 axis);

	// Getters
	float GetMass();
	Vector4 GetPosition();
	Vector4 GetVelocity();
	//Vector4 GetAcceleration();

	// Setters
	void SetMass(float mass);
	void SetPosition(Vector4 position);
	void SetVelocity(Vector4 velocity);
	//void SetAcceleration(Vector3 acceleration);
	void SetRotation(Vector4 rotation, float angle);

	void SetAngularVelocity(Vector4 velocity, float angle);
	

private:
	// options for now
	Vector4 p_totalForce;
	Vector4 p_totalAcceleration;
	// using vector instead of quaternion for now
	Vector4 p_orientation;
	Vector4 p_angularVelocity;
	float p_angle;

	// this is for physics assignment 1, will change everything to force after I think
	// might want to move parts of this to MathUtility.h
	void UpdateOrientationQuaternion(Vector4 angularVelocity, float angle);

};
#endif