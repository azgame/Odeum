#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

enum class ColliderType
{
	Box,
	Sphere,
	Complex,
	None
};

class ColliderComponent : public Component
{
public:

	virtual void OnAttach(GameObject* parent) override;
	virtual void OnDetach() {};
	virtual void Update(float deltaTime) {};

	ColliderType GetColliderType() { return m_type; }

	bool isTrigger = false;

protected:

	ColliderType m_type = ColliderType::None;
};


#endif