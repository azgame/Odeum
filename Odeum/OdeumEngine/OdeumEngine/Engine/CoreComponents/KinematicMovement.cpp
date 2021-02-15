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
			object->GetComponent<Rigidbody>()->SetPosition(object->GetComponent<Rigidbody>()->GetPosition() + temp);
		}
		else
		{
			temp = temp;
			object->GetComponent<Rigidbody>()->SetPosition(object->GetComponent<Rigidbody>()->GetPosition() - temp);
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
	velocity.SetX(target->GetComponent<Rigidbody>()->GetPosition().GetX() - object->GetComponent<Rigidbody>()->GetPosition().GetX());
	velocity.SetY(target->GetComponent<Rigidbody>()->GetPosition().GetY() - object->GetComponent<Rigidbody>()->GetPosition().GetY());
	velocity.SetZ(target->GetComponent<Rigidbody>()->GetPosition().GetZ() - object->GetComponent<Rigidbody>()->GetPosition().GetZ());
	velocity = velocity.Normalize();
	velocity= velocity * maxSpeed;
	//this deals with the rotation which will be implemented after
	if (velocity.Mag() > 0)
	{
		rotation=atan2(-velocity.GetX(), velocity.GetZ());
	}
	else
	{
		// JUST GOTTA FIGURE OUT HOW TO GET THIS FROM QUATERNION INSTEAD
		rotation = object->GetComponent<Rigidbody>()->GetRotation().GetY();
	}
	
	return Vector4(velocity, rotation);
}
