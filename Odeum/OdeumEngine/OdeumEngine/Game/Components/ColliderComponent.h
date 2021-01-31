#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

enum ColliderType
{
	Box,
	Sphere,
	Complex,
	None
};

class ColliderComponent : public Component
{
public:

	void OnAttach(GameObject* parent) override;
	void OnDetach() {};

	ColliderType GetColliderType() { return m_type; }

	bool isTrigger = false;

protected:

	ColliderType m_type = ColliderType::None;
};


#endif