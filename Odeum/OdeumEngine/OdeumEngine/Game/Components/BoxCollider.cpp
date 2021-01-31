#include "BoxCollider.h"
#include "Rigidbody.h"

BoxCollider::BoxCollider()
{
	m_type = ColliderType::Box;
}

void BoxCollider::OnAttach(GameObject* parent)
{
	ColliderComponent::OnAttach(parent);

	obb = new OrientedBoundingBox();
	obb->center = Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition());
}