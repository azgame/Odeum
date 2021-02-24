#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "../../pch.h"
#include "../Core/Component.h"
#include "Rigidbody.h"

class SphereCollider : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() {};
	void Update(float deltaTime) override;

	// Getters
	inline float GetRadius() { return sc_radius; }
	inline Vector3 GetPosition() { return sc_position; }
	inline Rigidbody* GetRigidbody() { return rb; }

	inline void SetRadius(float radius) { sc_radius = radius; }
	inline void SetPosition(Vector3 position) { sc_position = position; }

private:
	Rigidbody* rb;
	float sc_radius;
	Vector3 sc_position; // vector3 or vector4?
};
#endif