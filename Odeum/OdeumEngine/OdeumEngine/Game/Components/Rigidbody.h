#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class Rigidbody : public Component
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
	inline float GetMass() { return rb_mass; }
	inline Vector4 GetPosition() { return rb_position; }
	inline Vector4 GetVelocity() { return rb_totalVelocity; }
	inline Vector4 GetAcceleration() { return rb_totalAcceleration; }

	// Setters
	inline void SetMass(float mass) { rb_mass = mass; }
	inline void SetVelocity(Vector4 velocity) { rb_totalVelocity = velocity; }
	inline void SetAcceleration(Vector4 acceleration) { rb_totalAcceleration = acceleration; }
	inline void SetPosition(Vector4 position) { rb_position = position; UpdateTransform(); }
	inline void SetRotation(Vector4 rotation, float angle) { rb_orientation = Quaternion(Vector3(rotation), angle); UpdateTransform(); }
	inline void SetRotation(Quaternion rotationQuat) { rb_orientation = rotationQuat; UpdateTransform(); }

	inline void AddVelocity(Vector4 velocity) { rb_totalVelocity += velocity; }
	void AddAngularVelocity(Vector4 velocity, float angle);

private:
	// options for now
	Vector4 rb_totalForce;
	Vector4 rb_totalAcceleration;
	// using vector instead of quaternion for now
	Vector4 rb_position;
	//Vector4 rb_rotation;
	Vector4 rb_totalVelocity;
	Vector4 rb_angularVelocity;

	Quaternion rb_orientation;

	float rb_mass;
	float rb_angle;
	float rb_angleSpeed;


	float rb_speed;
	
	// this is for physics assignment 1, will change everything to force after I think
	// might want to move parts of this to MathUtility.h
	void UpdateOrientationQuaternion();

	// Cross product
	//inline Vector3 Cross(Vector3 v1, Vector3 v2)
	//{
	//	return Vector3(v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY(),
	//				   v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ(),
	//				   v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX());
	//}
	
	//inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_angle, rb_rotation, m_gameObject->GetScale()); }
	inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_orientation, m_gameObject->GetScale()); }
};

#endif