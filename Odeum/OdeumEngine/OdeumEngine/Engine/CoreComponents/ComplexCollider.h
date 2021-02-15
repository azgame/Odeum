#ifndef COMPLEXCOLLIDER_H
#define COMPLEXCOLLIDER_H

#include "../../pch.h"
#include "../Core/Component.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "../Math/Collider.h"

class ComplexCollider : public BoxCollider
{
public:

	ComplexCollider();

	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() {};
	void Update(float deltaTime) override;

	inline Vector3 GetPosition() { return Vector3(m_gameObject->GetPosition()); }
	inline Collider* GetCollider() { return collider; }

private:

	Collider* collider;
};

#endif