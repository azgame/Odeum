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
	//Vector4 GetAcceleration();

	// Setters
	void SetMass(float mass);

	void SetVelocity(Vector3 velocity);
	//Vector3 GetVelocity();
	// not sure which we want to keep in the physics component or the game object (ie. pos, vel, acc)
	void SetAcceleration(Vector4 acceleration );
	void SetPosition(Vector4 position);

	void AddAngularVelocity(Vector4 velocity, float angle);

	void AddVelocity(Vector4 velocity);
	

private:
	// options for now
	Vector4 p_totalForce;
	Vector4 p_totalAcceleration;
	// using vector instead of quaternion for now
	Vector4 p_orientation;
	Vector4 p_angularVelocity;
	float p_angle;
	float p_angleSpeed;

	Vector4 totalVelocity;

	float speed;
	
	// this is for physics assignment 1, will change everything to force after I think
	// might want to move parts of this to MathUtility.h
	Vector4 UpdateOrientationQuaternion();

	// Cross product
	inline Vector3 Cross(Vector3 v1, Vector3 v2)
	{
		return Vector3(v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY(),
					   v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ(),
					   v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX());
	}

};
#endif