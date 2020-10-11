#include "SimplePhysics.h"

void SimplePhysics::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	p_angle = 0.0f;
	p_angularVelocity = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	p_totalVelocity = Vector4();
	p_speed = 2.0f;

	p_position = m_gameObject->GetPosition();
	p_rotation = m_gameObject->GetRotation();
	p_scale = m_gameObject->GetScale();
	p_mass = m_gameObject->GetMass();
	p_orientation = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
}

// update position based on acceleration and velocity
void SimplePhysics::Update(float deltaTime)
{
	
	// update velocity
	p_totalVelocity += (p_totalAcceleration * deltaTime);
	p_position += p_totalVelocity * deltaTime +p_totalAcceleration*0.5f* deltaTime*deltaTime;

	// ******* CURRENTLY A PROBLEM WITH SETTING POSITION AND ROTATION
	//update position
	//m_gameObject->SetVelocity(m_gameObject->GetVelocity() + (Vector4)(totalAcceleration * deltaTime));
	//m_gameObject->SetPosition(m_gameObject->GetPosition() + (Vector4)(m_gameObject->GetVelocity() * deltaTime + totalAcceleration * 0.5f * deltaTime * deltaTime));
	// update velocity
	// update position
	p_rotation = p_orientation;
	// update orientation
	// this check just stops some math errors when there's no velocity set
	if (deltaTime > 0.0f) {
		// change game object orientation
		
		p_orientation = UpdateOrientationQuaternion();
		p_angle += p_angleSpeed;
	}
	//UpdateTransform();
	m_gameObject->SetPosition(p_position);
}

// translate the position
void SimplePhysics::Transform(Vector4 translate)
{
	p_position += translate;
	UpdateTransform();
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
	return p_mass;
}

Vector4 SimplePhysics::GetPosition()
{
	return p_position;

}
Vector4 SimplePhysics::GetVelocity()
{
	return p_totalVelocity;
}

// Setters
void SimplePhysics::SetMass(float mass)
{
	p_mass = mass;
}

void SimplePhysics::SetPosition(Vector4 position)
{
	p_position = position;
	UpdateTransform();
}

void SimplePhysics::SetVelocity(Vector4 velocity)
{
	p_totalVelocity = velocity;
}
void SimplePhysics::SetTotalAcceleration(Vector4 acceleration)
{
	p_totalAcceleration = acceleration;
}

void SimplePhysics::AddVelocity(Vector4 velocity)
{
	p_totalVelocity += velocity;
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
	Vector3 axisOfRotation_ = Vector3(Cross(Vector3(p_totalVelocity), Vector3(p_angularVelocity)));
	
	// normalize
	axisOfRotation_ = axisOfRotation_.Normalize();
	
	// calculate the magnitude of the angular velocity vector
	Vector3 angularVelocity_ = Vector3(axisOfRotation_ * p_totalVelocity.Mag());

	//Quaternion angVel(Vector4(angularVelocity_, p_angle));

	// update the orientation
	p_orientation = Vector4(angularVelocity_, p_angle);
	return p_orientation.Normalize();
}

void SimplePhysics::UpdateTransform() {
	m_gameObject->UpdateTransform(p_position, p_angle, p_rotation, p_scale);
}