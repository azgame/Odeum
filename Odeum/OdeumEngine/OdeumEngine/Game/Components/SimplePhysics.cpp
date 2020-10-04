#include "SimplePhysics.h"

void SimplePhysics::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	p_angle = 0.0f;
}

// update position based on acceleration and velocity
void SimplePhysics::Update(float deltaTime)
{
	// udpate orientation
	UpdateOrientationQuaternion(p_angularVelocity, p_angle);
	// change game object orientation
	m_gameObject->SetRotation(p_orientation, p_angle);

	// update velocity
	m_gameObject->SetVelocity(m_gameObject->GetVelocity() + (Vector4)(p_totalAcceleration * deltaTime));
	// update position
	m_gameObject->SetPosition(m_gameObject->GetPosition() + (Vector4)(m_gameObject->GetVelocity() * deltaTime + p_totalAcceleration * 0.5f * deltaTime * deltaTime));


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
	if (m_gameObject->GetMass() > 0)
	{
		acceleration = force / m_gameObject->GetMass();
	}

	// update total forces
	p_totalForce += force;
	p_totalAcceleration += acceleration;
}

// apply a force on an angle
void SimplePhysics::ApplyForce(Vector4 force, Vector4 axis)
{
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

void SimplePhysics::SetRotation(Vector4 rotation, float angle)
{
	m_gameObject->SetRotation(rotation, angle);
}

void SimplePhysics::SetAngularVelocity(Vector4 velocity, float angle)
{
	p_angularVelocity = velocity;
	p_angle = angle;
}

// might want to move parts of this to MathUtility.h
void SimplePhysics::UpdateOrientationQuaternion(Vector4 angularVelocity, float angle)
{
	// use cross product of the angular velocity and the current velocity to find axis of rotation
	Vector3 axisOfRotation_ = Vector3(Math::Cross(angularVelocity, m_gameObject->GetVelocity()));
	
	// normalize
	axisOfRotation_ = axisOfRotation_.Normalize();
	
	// calculate the magnitude of the angular velocity vector
	Vector3 angularVelocity_ = Vector3(angularVelocity.Mag() * axisOfRotation_);

	// update the orientation
	p_orientation += p_orientation * angularVelocity_ * 0.5f;
}
