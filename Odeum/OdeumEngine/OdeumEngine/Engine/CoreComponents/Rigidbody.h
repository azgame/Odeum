#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../../pch.h"
#include "../Core/Component.h"

class Rigidbody : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() {};
	void Update(float deltaTime) override;

	// movement functions
	void Transpose(Vector4 translate);
	void ApplyForce(Vector4 force);
	void ApplyTorque(Vector4 force);
	inline void AddVelocity(Vector4 velocity) { rb_totalVelocity += velocity; }
	void AddAngularVelocity(Vector4 velocity);
	void AddAngularVelocity(Vector4 velocity, float angleSpeed);

	// Getters
	inline float GetMass() { return rb_mass; }
	inline Vector4 GetPosition() { return rb_position; }
	inline Vector4 GetVelocity() { return rb_totalVelocity; }
	inline Vector4 GetAngularVelocity() { return rb_totalAngularVelocity; }
	inline Vector4 GetAcceleration() { return rb_totalAcceleration; }
	inline Vector4 GetScale() { return rb_scale; }
	inline float GetRadius() { return rb_radius; }
	inline float GetMomentOfInertiaSphere() { return rb_momentOfInertiaSphere; }
	inline float GetRotationSpeed() { return rb_rotationSpeed; }


	// gotta change this
	//inline Vector4 GetRotation() { return Vector4(rb_orientation); }
	//inline Quaternion GetOrientation() { return rb_orientation; }
	inline Matrix4 GetRotation() { return rb_rotation; }
	inline Matrix3 GetInvInertiaTensor() { return rb_inertiaTensorInv; }

	// Setters
	inline void SetMass(float mass) { rb_mass = mass; }
	inline void SetVelocity(Vector4 velocity) { rb_totalVelocity = velocity;  }
	inline void SetAngularVelocity(Vector4 angularVelocity, float rotationSpeed) { rb_totalAngularVelocity = angularVelocity; rb_rotationSpeed = rotationSpeed; }
	inline void SetAcceleration(Vector4 acceleration) { rb_totalAcceleration = acceleration; }
	inline void SetPosition(Vector4 position) { rb_position = position; }//UpdateTransform(); }
	//inline void SetRotation(Vector4 rotation, float angle) { if (Vector3(rotation).Mag() > 0.0f) rb_rotation = Matrix4(DirectX::XMMatrixRotationAxis(rotation, angle)); } //UpdateTransform(); }
	inline void SetRotation(Vector4 rotation, float angle) { if (Vector3(rotation).Mag() > 0.0f) rb_orientation = Quaternion(Vector3(rotation), angle); } //UpdateTransform(); }
	//inline void SetRotation(Quaternion rotationQuat) { rb_orientation = rotationQuat; } //UpdateTransform();}
	//inline void SetRotation(Quaternion rotationQuat, float rotSpeed) { rb_orientation = rotationQuat; rb_rotationSpeed = rotSpeed; } //UpdateTransform();}
	inline void SetScale(Vector4 scale) { rb_scale = scale; }
	inline void SetRadius(float radius) { rb_radius = radius; }

	//inline std::string GetPositionString() { return "string"; }



private:
	// Member Variables
	Vector4 rb_totalForce;
	Vector4 rb_totalAcceleration;
	Vector4 rb_totalTorque;
	Vector4 rb_position;
	Matrix4 rb_rotation; // the axis in which the object will be rotating around
	Vector4 rb_totalVelocity;
	Vector4 rb_totalAngularVelocity;
	Vector4 rb_scale;
	Quaternion rb_orientation;

	Matrix3 rb_inertiaTensor;
	Matrix3 rb_inertiaTensorInv;
	float rb_momentOfInertiaSphere;

	float rb_radius;

	float rb_mass;
	float rb_angle;

	float rb_angleSpeed;
	float rb_rotationSpeed;

	float rb_speed;

	float rb_massDensity;
	float rb_volume;

	//void UpdateOrientationQuaternion();

	void UpdateOrientation(float deltaTime);

	void UpdateInertiaTensor();

	//inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_angle, rb_rotation, rb_scale); }
	//inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_rotation, rb_scale); }
	inline void UpdateTransform() { m_gameObject->UpdateTransform(rb_position, rb_orientation, rb_scale); }
};

#endif