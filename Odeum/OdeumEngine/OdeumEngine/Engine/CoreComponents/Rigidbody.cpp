#include "Rigidbody.h"

void Rigidbody::OnAttach(GameObject* parent)
{
	m_gameObject = parent;

	rb_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	rb_angle = 0.0f;
	rb_orientation = Quaternion();
	rb_rotation = Matrix4();
	rb_totalAngularVelocity = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	rb_totalVelocity = Vector4();
	rb_speed = 2.0f;
	rb_angleSpeed = 1.0f;
	rb_rotationSpeed = 1.0f;

	rb_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	rb_mass = 1.0f;

	rb_massDensity = 1.0f;
	rb_volume = 1.0f;
}

// update position based on acceleration and velocity
void Rigidbody::Update(float deltaTime)
{
	// update velocity
	rb_totalVelocity += rb_totalAcceleration * deltaTime;

	//update position
	rb_position += rb_totalVelocity * deltaTime + rb_totalAcceleration * 0.5f * deltaTime * deltaTime;

	rb_angle += rb_rotationSpeed;

	// Angular Velocity stuff
	//rb_totalAngularVelocity += Vector4(rb_inertiaTensorInv.MultiplyWithVec(Vector3(rb_totalTorque)));
	
	/*if (Vector3(rb_totalAngularVelocity).Mag() > 0) {
		rb_rotation = rb_rotation * Matrix4(DirectX::XMMatrixRotationAxis(Vector3(rb_totalAngularVelocity), rb_rotationSpeed * deltaTime));
		// try multiplying by 3 matrices
		//rb_rotation = rb_rotation * Matrix4(DirectX::XMMatrixRotationAxis(Vector3(rb_totalAngularVelocity), rb_rotationSpeed * deltaTime));
	}*/
	
	// update orientation
	//UpdateOrientationQuaternion();
	if (rb_totalAngularVelocity.Mag() > 0.0f)
	{
		UpdateOrientation(deltaTime);
	}
	// Update Transform
	UpdateTransform();

	// Update Inertia Tensor
	UpdateInertiaTensor();
}


void Rigidbody::UpdateOrientation(float deltaTime)
{
	//rb_orientation = Quaternion(rb_orientation * deltaTime + (deltaTime / 2) * rb_totalAngularVelocity * rb_orientation * deltaTime);
	//rb_orientation = Quaternion(rb_orientation + (rb_totalAngularVelocity / 2) * rb_orientation);
	//rb_orientation = Quaternion(rb_orientation.GetVector4().Normalize());
	
	rb_orientation = rb_orientation * Quaternion(rb_totalAngularVelocity, rb_rotationSpeed);
	rb_orientation = Quaternion(rb_orientation.GetVector4().Normalize());
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

void Rigidbody::ApplyTorque(Vector4 force)
{
	rb_totalTorque += force;
}

void Rigidbody::AddAngularVelocity(Vector4 velocity)
{
	rb_totalAngularVelocity += velocity;
}

void Rigidbody::AddAngularVelocity(Vector4 velocity, float rotationSpeed)
{
	rb_totalAngularVelocity += velocity;
	rb_rotationSpeed = rotationSpeed;
}


/*void Rigidbody::UpdateOrientationQuaternion()
{
	// use cross product of the up vector vv (STILL NEED TO GET THIS) velocity to find axis of rotation
	//Vector3 axisOfRotation_ = Math::Cross(Vector3(rb_totalVelocity), Vector3(rb_angularVelocity));
	Vector3 axisOfRotation_ = Vector3(rb_angularVelocity);

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
}*/


// this should be changed to differ based on the shape - this is just a basic version
void Rigidbody::UpdateInertiaTensor()
{
	rb_inertiaTensor.SetX(Vector3(
		(rb_position.GetZ() * rb_position.GetZ() + rb_position.GetY() * rb_position.GetY()) * rb_massDensity * rb_volume,
		-(rb_position.GetY() * rb_position.GetX()) * rb_massDensity * rb_volume,
		-(rb_position.GetZ() * rb_position.GetX()) * rb_massDensity * rb_volume));
	rb_inertiaTensor.SetY(Vector3(
		-(rb_position.GetX() * rb_position.GetY()) * rb_massDensity * rb_volume,
		(rb_position.GetZ() * rb_position.GetZ() + rb_position.GetX() * rb_position.GetX()) * rb_massDensity * rb_volume,
		-(rb_position.GetZ() * rb_position.GetY()) * rb_massDensity * rb_volume));
	rb_inertiaTensor.SetZ(Vector3(
		-(rb_position.GetX() * rb_position.GetZ()) * rb_massDensity * rb_volume,
		-(rb_position.GetY() * rb_position.GetZ()) * rb_massDensity * rb_volume,
		(rb_position.GetX() * rb_position.GetX() + rb_position.GetY() * rb_position.GetY()) * rb_massDensity * rb_volume));

	rb_inertiaTensorInv = Matrix3(DirectX::XMMatrixInverse(nullptr, rb_inertiaTensor));
	rb_momentOfInertiaSphere = (2.0f / 5.0f) * rb_mass * rb_radius * rb_radius;
}
