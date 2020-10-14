#include "DynamicMovement.h"
void DynamicMovement::OnAttach(GameObject* parent)
{
	object = parent;
	//change these values later these are placeholders, should be set depending on the object
	 maxAcceleration=5;
	 maxSpeed=5;
	 targetRadius=1.0f;
	 slowRadius=10;
	 timeToTarget=0.1;
}

void DynamicMovement::Update(float deltaTime)
{
	//requires target to work
	if (target)
	{
		if(!shouldFlee)
		object->GetComponent<SimplePhysics>()->SetAcceleration(GetSteering());
		else
		object->GetComponent<SimplePhysics>()->SetAcceleration(-GetSteering());
		
			
	}
}

Vector4 DynamicMovement::GetSteering()
{
	Vector3 temp;
	Vector3 direction;
	//direction=target position-object position
	direction.SetX(target->GetPosition().GetX() - object->GetPosition().GetX());
	direction.SetY(target->GetPosition().GetY() - object->GetPosition().GetY());
	direction.SetZ(target->GetPosition().GetZ() - object->GetPosition().GetZ());
	float distance = direction.Mag();
	float targetSpeed;
	//stop acclerating when in range
	if (distance < targetRadius)
	{
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	//if not in the slow range go full force otherwise slow down
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

	return Vector4(temp.GetX(),0.0f,temp.GetZ(), 0.0f);
}