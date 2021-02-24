#include "SphereCollider.h"

void SphereCollider::OnAttach(GameObject* parent) {
	m_gameObject = parent;
	rb = parent->GetComponent<Rigidbody>();
	sc_radius = rb->GetRadius();
	sc_position = Vector3(rb->GetPosition());
}

void SphereCollider::Update(float deltaTime) {
	sc_position = Vector3(rb->GetPosition());
}