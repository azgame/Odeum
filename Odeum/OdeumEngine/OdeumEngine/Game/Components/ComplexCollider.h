#ifndef COMPLEXCOLLIDER_H
#define COMPLEXCOLLIDER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../Game/Components/Rigidbody.h"
#include "../Engine/Math/Collider.h"

class ComplexCollider : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	// Getters
	inline Vector3 GetPosition() { return cc_position; }
	inline Rigidbody* GetRigidbody() { return rb; }
	inline Collider* GetCollider() { return collider; }

	inline void SetPosition(Vector3 position) { cc_position = position; }
	inline void SetCollider(Collider* collider_) { collider = collider_; }

private:
	Rigidbody* rb;
	Vector3 cc_position; // vector3 or vector4?

	Collider* collider;
};

#endif