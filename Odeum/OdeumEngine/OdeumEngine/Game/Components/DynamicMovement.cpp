#include "DynamicMovement.h"
void DynamicMovement::OnAttach(GameObject* parent)
{

}
void DynamicMovement::Update(float deltaTime)
{

}
Vector4 DynamicMovement::GetSteering()
{
	Vector3 temp;
	Vector3 direction;
	direction.SetX(target->GetPosition().GetX() - object->GetPosition().GetX());
	direction.SetY(target->GetPosition().GetY() - object->GetPosition().GetY());
	direction.SetZ(target->GetPosition().GetZ() - object->GetPosition().GetZ());
	float distance = direction.Mag();
	float targetSpeed;
	if (distance < targetRadius)
	{
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	if (distance > slowRadius)
	{
		targetSpeed = maxSpeed;
	}
	else
	{
	
		
		targetSpeed = maxSpeed * distance / slowRadius;
	}
	
	direction=direction.Normalize();
	direction=direction * targetSpeed;
	temp = direction - object->GetComponent<SimplePhysics>()->GetVelocity();
	temp = temp / timeToTarget;
	// Clip acceleration if too high
	if (temp.Mag() > maxAcceleration)
	{
		temp = temp.Normalize();
		temp = temp * maxAcceleration;
	}


		return Vector4(temp, 0.0f);
}