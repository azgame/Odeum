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

class ColliderComponent : Component
{
public:
	ColliderType GetColliderType() { return m_type; }

	bool isTrigger = false;

private:

	ColliderType m_type = ColliderType::None;
};


#endif