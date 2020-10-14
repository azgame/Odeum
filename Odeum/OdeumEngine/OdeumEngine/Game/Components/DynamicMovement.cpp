#include "DynamicMovement.h"
void DynamicMovement::OnAttach(GameObject* parent)
{
	object = parent;
	//if (object->GetComponent<SimplePhysics>()){}
	//else { object->AddComponent<SimplePhysics>(); Debug::Info("add comp", __FILENAME__, __LINE__);
	//}
	

	
	//change these values later these are placeholders, should be set depending on the object
	 maxAcceleration=5;
	 maxSpeed=5;
	 targetRadius=3.0f;
	 slowRadius=6;
	 timeToTarget=0.1f;
}

void DynamicMovement::Update(float deltaTime)
{
	//requires target to work
	if (target)
	{
		
		
		object->GetComponent<SimplePhysics>()->SetTotalAcceleration(GetSteering());

		
			
	}
}
void DynamicMovement::SetAll(float maxAcceleration_, float maxSpeed_, float targetRadius_, float slowRadius_, float timeToTarget_)
{
	maxAcceleration = maxAcceleration_; maxSpeed = maxSpeed_; targetRadius = targetRadius_; slowRadius = slowRadius_; timeToTarget = targetRadius_;
}
Vector4 DynamicMovement::GetSteering()
{
	Vector3 temp;
	Vector3 direction;
	//direction=target position-object position
	if (!shouldFlee)
	{
		direction.SetX(target->GetPosition().GetX() - object->GetPosition().GetX());
		direction.SetY(target->GetPosition().GetY() - object->GetPosition().GetY());
		direction.SetZ(target->GetPosition().GetZ() - object->GetPosition().GetZ());
	}
	else
	{
		direction.SetX(object->GetPosition().GetX() - target->GetPosition().GetX());
		direction.SetY(object->GetPosition().GetY() - target->GetPosition().GetY());
		direction.SetZ(object->GetPosition().GetZ() - target->GetPosition().GetZ());
	}
	float distance = direction.Mag();
	
	float targetSpeed;
	//stop acclerating when in range
	if (distance < targetRadius&&!shouldFlee)
	{
		
			object->GetComponent<SimplePhysics>()->SetVelocity(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
			return(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
		
	
		
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