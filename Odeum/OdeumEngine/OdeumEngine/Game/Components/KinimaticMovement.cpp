#include "KinimaticMovement.h"
void KinimaticMovement::Update(float deltaTime)
{
	//object->SetPosition(object->GetPosition() + Vector4(0.01f, 0.0f, 0.0f, 0.0f));
	// Component does stuff
	if (target)
	{
		Debug::Info("target aquired", __FILENAME__, __LINE__);
		Vector4 temp = GetSteering();
		
	
		object->SetPosition(object->GetPosition() + temp);
		physicsComp->ApplyForce(Vector4(temp.GetX(), temp.GetY(), temp.GetZ(), 1.0f));
	}
}
void KinimaticMovement::OnAttach(GameObject* parent)
{
	object = parent;
	if (maxSpeed < 0.01)
	{
		maxSpeed = 0.01;
	}
    physicsComp=	object->GetComponent<SimplePhysics>();
	
}
Vector4 KinimaticMovement::GetSteering()
{
	Vector3 velocity(0.0f, 0.0f, 0.0f);
	
	float rotation;
	velocity.SetX(target->GetPosition().GetX() - object->GetPosition().GetX());
	velocity.SetY(target->GetPosition().GetY() - object->GetPosition().GetY());
	velocity.SetZ(target->GetPosition().GetZ() - object->GetPosition().GetZ());
	velocity = velocity.Normalize();
	velocity= velocity * maxSpeed;
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
