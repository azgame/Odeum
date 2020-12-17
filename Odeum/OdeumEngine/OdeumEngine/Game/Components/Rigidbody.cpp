#include "Rigidbody.h"

void Rigidbody::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	rb_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	rb_angle = 0.0f;
	rb_orientation = Quaternion();
	rb_angularVelocity = Vector4();
	rb_totalVelocity = Vector4();
	rb_speed = 2.0f;
	rb_angleSpeed = 0.0005f;
	rb_rotationSpeed = 0.0005f;

	rb_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

// update position based on acceleration and velocity
void Rigidbody::Update(float deltaTime)
{
	
	// update velocity
	rb_totalVelocity += rb_totalAcceleration * deltaTime;
	
	//update position
	rb_position += rb_totalVelocity * deltaTime + rb_totalAcceleration * 0.5f * deltaTime * deltaTime;
	
	rb_angle += rb_angleSpeed;

	// update orientation
	UpdateOrientationQuaternion();

	// Update Transform
	UpdateTransform();
}

// translate the position
void Rigidbody::Transpose(Vector4 translate)
{
	rb_position += translate;
}

// apply a force on the object
void Rigidbody::ApplyForce(Vector4 force)
{
	if (rb_mass > 0)
	{
		rb_totalAcceleration = force / rb_mass;
	}

	// update total forces
	rb_totalForce += force;

}

void Rigidbody::AddAngularVelocity(Vector4 velocity)
{
	rb_angularVelocity += velocity;
}

void Rigidbody::AddAngularVelocity(Vector4 velocity, float angleSpeed)
{
	rb_angularVelocity += velocity;
	rb_angleSpeed = angleSpeed;
}


void Rigidbody::UpdateOrientationQuaternion()
{
	// use cross product of the up vector vv (STILL NEED TO GET THIS) velocity to find axis of rotation
	Vector3 axisOfRotation_ = Math::Cross(Vector3(rb_totalVelocity), Vector3(rb_angularVelocity));
	
	// normalize
	if (axisOfRotation_.Mag() == 0)
	{
		return;
	}
	axisOfRotation_ = axisOfRotation_.Normalize();
	
	// calculate the magnitude of the angular velocity vector
	Vector3 angularVelocity_ = Vector3(axisOfRotation_ * rb_angularVelocity.Mag());

	Quaternion angVel(Vector4(angularVelocity_, rb_angleSpeed));

	// update the orientation
	rb_orientation = Quaternion(Vector4(rb_orientation + (rb_orientation * angVel * 0.5f * rb_angleSpeed)).Normalize());
}
