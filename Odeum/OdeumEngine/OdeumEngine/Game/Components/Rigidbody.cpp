#include "Rigidbody.h"

void Rigidbody::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	rb_position = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	//rb_rotation = Vector4(0.0f, 1.0f, 0.0f, 0.0f);


	rb_angle = 0.0f;
	rb_orientation = Quaternion();
	rb_angularVelocity = Vector4();
	rb_totalVelocity = Vector4();
	rb_speed = 2.0f;
	rb_angleSpeed = 1.0f;

}

// update position based on acceleration and velocity
void Rigidbody::Update(float deltaTime)
{
	
	// update velocity
	//totalVelocity += (Vector4)(p_totalAcceleration * deltaTime);
	rb_totalVelocity += (rb_totalAcceleration * deltaTime);
	rb_position += rb_totalVelocity * deltaTime + rb_totalAcceleration * 0.5f * deltaTime * deltaTime;
	// ******* CURRENTLY A PROBLEM WITH SETTING POSITION AND ROTATION
	//update position
	//rb_position += rb_totalVelocity * rb_speed * deltaTime;
	//m_gameObject->SetPosition(m_gameObject->GetPosition() + (Vector4)(totalVelocity * deltaTime + p_totalAcceleration * 0.5f * deltaTime * deltaTime));

	// update orientation
	// this check just stops some math errors when there's no velocity set
	// change game object orientation
	//m_gameObject->SetRotation(m_gameObject->GetRotation() + UpdateOrientationQuaternion(), p_angle);
	rb_angle += rb_angleSpeed;

	//UpdateOrientationQuaternion();
	m_gameObject->SetPosition(rb_position);
	//UpdateTransform();
}

// translate the position
void Rigidbody::Transform(Vector4 translate)
{
	m_gameObject->SetPosition(m_gameObject->GetPosition() + translate);
}

// apply a force on the object
void Rigidbody::ApplyForce(Vector4 force)
{
	Vector4 acceleration;
	if (m_gameObject->GetMass() > 0)
	{
		acceleration = force / m_gameObject->GetMass();
	}

	// update total forces
	rb_totalForce += force;
	rb_totalAcceleration += acceleration;
}

void Rigidbody::AddAngularVelocity(Vector4 velocity, float angle)
{
	rb_angularVelocity += velocity;
	//rb_totalVelocity += velocity;
	rb_angleSpeed = angle;
}

// might want to move parts of this to MathUtility.h
void Rigidbody::UpdateOrientationQuaternion()
{
	// use cross product of the up vector vv (STILL NEED TO GET THIS) velocity to find axis of rotation
	Vector3 axisOfRotation_ = Vector3(Cross(Vector3(rb_totalVelocity), Vector3(rb_angularVelocity)));
	
	// normalize
	if (axisOfRotation_.Mag() == 0)
	{
		return;
	}
	axisOfRotation_ = axisOfRotation_.Normalize();
	
	// calculate the magnitude of the angular velocity vector
	Vector3 angularVelocity_ = Vector3(axisOfRotation_ * rb_angularVelocity.Mag());

	Quaternion angVel(Vector4(angularVelocity_, 0.0005f));

	// update the orientation
	rb_orientation = Quaternion(rb_orientation + (rb_orientation * angVel * 0.5f * rb_angleSpeed));
	rb_orientation = Quaternion(rb_orientation.GetVector4().Normalize());
}
