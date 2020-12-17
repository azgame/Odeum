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

	// movement functions
	void Transpose(Vector4 translate);
	void ApplyForce(Vector4 force);
	inline void AddVelocity(Vector4 velocity) { rb_totalVelocity += velocity; }
	void AddAngularVelocity(Vector4 velocity);
	void AddAngularVelocity(Vector4 velocity, float angleSpeed);

	// Getters
	inline float GetMass() { return rb_mass; }
	inline Vector4 GetPosition() { return rb_position; }
	inline Vector4 GetVelocity() { return rb_totalVelocity; }
	inline Vector4 GetAcceleration() { return rb_totalAcceleration; }
	inline Vector4 GetScale() { return rb_scale; }
	inline float GetRadius() { return rb_radius; }

	// gotta change this
	inline Vector4 GetRotation() { return Vector4(rb_orientation); }

	// Setters
	inline void SetMass(float mass) { rb_mass = mass; }
	inline void SetVelocity(Vector4 velocity) { rb_totalVelocity = velocity; }
	inline void SetAcceleration(Vector4 acceleration) { rb_totalAcceleration = acceleration; }
	inline void SetPosition(Vector4 position) { rb_position = position; }//UpdateTransform(); }
	inline void SetRotation(Vector4 rotation, float angle) { if(Vector3(rotation).Mag() > 0.0f) rb_orientation = Quaternion(Vector3(rotation), angle); } //UpdateTransform(); }
	inline void SetRotation(Vector4 rotation, float angle, float rotSpeed) { if (Vector3(rotation).Mag() > 0.0f) rb_orientation = Quaternion(Vector3(rotation), angle); rb_rotationSpeed = rotSpeed; } //UpdateTransform(); }
	inline void SetRotation(Quaternion rotationQuat) { rb_orientation = rotationQuat; } //UpdateTransform();}
	inline void SetRotation(Quaternion rotationQuat, float rotSpeed) { rb_orientation = rotationQuat; rb_rotationSpeed = rotSpeed; } //UpdateTransform();}
	inline void SetScale(Vector4 scale) { rb_scale = scale; }
	inline void SetRadius(float radius) { rb_radius = radius; }
	

private:
	// Member Variables
	Vector4 rb_totalForce;
	Vector4 rb_totalAcceleration;
	Vector4 rb_position;
	//Vector4 rb_rotation;
	Vector4 rb_totalVelocity;
	Vector4 rb_angularVelocity;
	Vector4 rb_scale;
	Quaternion rb_orientation;
	float rb_radius;

	float rb_mass;
	float rb_angle;
	
	float rb_angleSpeed;
	float rb_rotationSpeed;

	float rb_speed;
	
	void UpdateOrientationQuaternion();
	
	//inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_angle, rb_rotation, m_gameObject->GetScale()); }
	inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_orientation, rb_scale); }
};

#endif