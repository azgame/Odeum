#include "BoxCollider.h"
#include "Rigidbody.h"

void BoxCollider::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	obb = new OrientedBoundingBox();
	obb->center = Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition());
}