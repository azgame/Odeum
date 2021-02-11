#include "BoxCollider.h"
#include "Rigidbody.h"
#include "RenderComponent.h"


void BoxCollider::OnAttach(GameObject* parent)
{
	m_type = ColliderType::Box;

	ColliderComponent::OnAttach(parent);

	if (m_gameObject->HasComponent<RenderComponent>())
	{
		obb = &m_gameObject->GetComponent<RenderComponent>()->GetBoundingBox();
	}
	else
	{
		obb = new OrientedBoundingBox();
	}

	obb->center = Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition());
}

void BoxCollider::Update(float deltaTime)
{
	obb->basis = Matrix3(m_gameObject->GetTransform().Get3x3());
	obb->center = Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition());
}
