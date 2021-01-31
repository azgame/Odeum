#include "ComplexCollider.h"

ComplexCollider::ComplexCollider()
{
	m_type = ColliderType::Complex;
}

void ComplexCollider::OnAttach(GameObject* parent) 
{	
	BoxCollider::OnAttach(parent);

	// default to box collider for now
	std::vector<Vector3> vertices;
	Vector3 nextVertex = Vector3(-1.0f, -1.0f, -1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(-1.0f, -1.0f, 1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(1.0f, -1.0f, -1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(1.0f, -1.0f, 1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(-1.0f, 1.0f, -1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(-1.0f, 1.0f, 1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(1.0f, 1.0f, -1.0f);
	vertices.push_back(nextVertex);
	nextVertex = Vector3(1.0f, 1.0f, 1.0f);
	vertices.push_back(nextVertex);
	
	collider = new Collider(vertices); 

	// make sure they are up to date with the rigidbody position and orientatino
	collider->Rotate(Quaternion(m_gameObject->GetTransform().Get3x3()));
	collider->Transpose(Vector3(m_gameObject->GetTransform().GetW()));
}

void ComplexCollider::Update(float deltaTime) 
{
	// this should keep the colliders up to date
	collider->Rotate(Quaternion(m_gameObject->GetTransform().Get3x3()));
	collider->Transpose(Vector3(m_gameObject->GetTransform().GetW()));
	
}