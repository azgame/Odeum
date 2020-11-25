#include "BoxCollider.h"

void BoxCollider::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	obb = new OrientedBoundingBox();
}