#include "ComplexCollider.h"

void ComplexCollider::OnAttach(GameObject* parent) {
	m_gameObject = parent;
	rb = parent->GetComponent<Rigidbody>();
	cc_position = Vector3(rb->GetPosition());
	
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
	//collider->Rotate(rb->GetOrientation()); 
	collider->Rotate(rb->GetRotation());
	collider->Transpose(cc_position);
	

}

void ComplexCollider::Update(float deltaTime) {
	cc_position = Vector3(rb->GetPosition());
	
	// this should keep the colliders up to date
	//collider->Rotate(rb->GetOrientation()); 
	collider->Rotate(rb->GetRotation());
	collider->Transpose(cc_position);
	
}