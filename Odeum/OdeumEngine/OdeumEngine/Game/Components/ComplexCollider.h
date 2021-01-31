#ifndef COMPLEXCOLLIDER_H
#define COMPLEXCOLLIDER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../Game/Components/Rigidbody.h"
#include "../Game/Components/BoxCollider.h"
#include "../Engine/Math/Collider.h"

class ComplexCollider : public BoxCollider
{
public:

	ComplexCollider();

	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	inline Vector3 GetPosition() { return Vector3(m_gameObject->GetTransform().GetX()); }
	inline Collider* GetCollider() { return collider; }

private:

	Collider* collider;
};

#endif