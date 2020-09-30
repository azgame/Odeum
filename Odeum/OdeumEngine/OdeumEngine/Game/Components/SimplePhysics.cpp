#include "SimplePhysics.h"

void SimplePhysics::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	m_mass = 1.0f;
}

// update position based on acceleration and velocity
void SimplePhysics::Update(float deltaTime)
{
	// update velocity
	m_velocity += m_acceleration * deltaTime;
	// update position
	m_position += m_velocity * deltaTime + m_acceleration * 0.5f * deltaTime * deltaTime;
}

// translate the position
void SimplePhysics::Transform(Vector3 translate)
{
	m_gameObject->SetPosition(m_gameObject->GetPosition() + translate);
}

// apply a force on the object
void SimplePhysics::ApplyForce(Vector3 force)
{
	m_acceleration = force / m_mass;
}

void SimplePhysics::SetMass(float mass)
{
	m_mass = mass;
}
