#include "SimplePhysics.h"

void SimplePhysics::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	p_angle = 0.0f;
	p_orientation = m_gameObject->GetRotation();
	p_angularVelocity = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	totalVelocity = Vector4();
	speed = 2.0f;

}

// update position based on acceleration and velocity
void SimplePhysics::Update(float deltaTime)
{
	
	// update velocity
	//totalVelocity += (Vector4)(p_totalAcceleration * deltaTime);

	// ******* CURRENTLY A PROBLEM WITH SETTING POSITION AND ROTATION
	//update position
	//m_gameObject->SetPosition(m_gameObject->GetPosition() + Vector4(totalVelocity * speed * deltaTime));
	//m_gameObject->SetPosition(m_gameObject->GetPosition() + (Vector4)(totalVelocity * deltaTime + p_totalAcceleration * 0.5f * deltaTime * deltaTime));
	

	// update orientation
	// this check just stops some math errors when there's no velocity set
	if (deltaTime > 0.0f) {
		// change game object orientation
		m_gameObject->SetRotation(m_gameObject->GetRotation() + UpdateOrientationQuaternion(), p_angle);
		p_angle += p_angleSpeed;
	}
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

// Getters
float SimplePhysics::GetMass()
{
	return m_gameObject->GetMass();
}

Vector4 SimplePhysics::GetPosition()
{
	return m_gameObject->GetPosition();
}

// Setters
void SimplePhysics::SetMass(float mass)
{
	m_gameObject->SetMass(mass);
}

void SimplePhysics::SetVelocity(Vector3 velocity)
{
}

void SimplePhysics::SetAcceleration(Vector4 acceleration)
{
	p_totalAcceleration = acceleration;
}

void SimplePhysics::SetPosition(Vector4 position)
{
	m_gameObject->SetPosition(position);
}

void SimplePhysics::AddVelocity(Vector4 velocity)
{
	totalVelocity = velocity;
}

void SimplePhysics::AddAngularVelocity(Vector4 velocity, float angle)
{
	p_angularVelocity = velocity;
	p_angleSpeed = angle;
}

// might want to move parts of this to MathUtility.h
Vector4 SimplePhysics::UpdateOrientationQuaternion()
{
	// use cross product of the up vector vv (STILL NEED TO GET THIS) velocity to find axis of rotation
	Vector3 axisOfRotation_ = Vector3(Cross(Vector3(totalVelocity), Vector3(p_angularVelocity)));
	
	// normalize
	axisOfRotation_ = axisOfRotation_.Normalize();
	
	// calculate the magnitude of the angular velocity vector
	Vector3 angularVelocity_ = Vector3(axisOfRotation_ * totalVelocity.Mag());

	Quaternion angVel(angularVelocity_, p_angle);

	// update the orientation
	p_orientation += p_orientation * angVel * 0.5f;
	return p_orientation.Normalize();
}