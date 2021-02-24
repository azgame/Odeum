#include "ColliderComponent.h"

void ColliderComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}
