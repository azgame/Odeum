#include "KinematicMovement.h"

void KinematicMovement::Update(float deltaTime)
{
	//make sure we have a target or there will be errors
	if (target)
	{
		
		Vector4 temp = GetSteering();
		
	    //sets position directly moving constantly 
		if (!shouldFlee)
		{
			object->SetPosition(object->GetPosition() + temp);
		}
		else
		{
			temp = temp;
			object->SetPosition(object->GetPosition() - temp);
		}
		
		
	}
}

void KinematicMovement::OnAttach(GameObject* parent)
{   //set default values and get our object's components
	object = parent;
	if (maxSpeed < 0.01)
	{
		maxSpeed = 0.01;
	}
}

Vector4 KinematicMovement::GetSteering()
{
	Vector3 velocity(0.0f, 0.0f, 0.0f);
	
	float rotation;
	//velocity=target position -object position
	velocity.SetX(target->GetPosition().GetX() - object->GetPosition().GetX());
	velocity.SetY(target->GetPosition().GetY() - object->GetPosition().GetY());
	velocity.SetZ(target->GetPosition().GetZ() - object->GetPosition().GetZ());
	velocity = velocity.Normalize();
	velocity= velocity * maxSpeed;
	//this deals with the rotation which will be implemented after
	if (velocity.Mag() > 0)
	{
		rotation=atan2(-velocity.GetX(), velocity.GetZ());
	}
	else
	{
		rotation = object->GetRotation().GetY();
	}
	
	return Vector4(velocity, rotation);
}
