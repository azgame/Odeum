#include "SimplePhysics.h"

void SimplePhysics::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}

// update position based on acceleration and velocity
void SimplePhysics::Update(float deltaTime)
{
	// update velocity
	m_gameObject->SetVelocity(m_gameObject->GetVelocity() + (Vector4)(totalAcceleration * deltaTime));
	// update position
	m_gameObject->SetPosition(m_gameObject->GetPosition() + (Vector4)(m_gameObject->GetVelocity() * deltaTime + totalAcceleration * 0.5f * deltaTime * deltaTime));
}

// translate the position
void SimplePhysics::Transform(Vector4 translate)
{
	m_gameObject->SetPosition(m_gameObject->GetPosition() + translate);
}

// apply a force on the object
void SimplePhysics::ApplyForce(Vector4 force)
{
	Vector4 acceleration;
	acceleration = force / m_gameObject->GetMass();

	// update total forces
	totalForce += force;
	totalAcceleration += acceleration;
}

// Getters

float SimplePhysics::GetMass()
{
	return m_gameObject->GetMass();
}

Vector4 SimplePhysics::GetPosition()
{
	return m_gameObject->GetPosition();

}

Vector4 SimplePhysics::GetVelocity()
{
	return m_gameObject->GetVelocity();
}

// Setters
void SimplePhysics::SetMass(float mass)
{
	m_gameObject->SetMass(mass);
}

void SimplePhysics::SetPosition(Vector4 position)
{
	m_gameObject->SetPosition(position);
}

void SimplePhysics::SetVelocity(Vector4 velocity)
{
	m_gameObject->SetVelocity(velocity);
}