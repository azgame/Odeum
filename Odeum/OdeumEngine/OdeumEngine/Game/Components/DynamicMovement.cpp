#include "DynamicMovement.h"
#include "../Engine/Math/CollisionDetection.h"
#include "../Engine/Math/CollisionHandler.h"
#include "../Engine/Math/Ray.h"
void DynamicMovement::OnAttach(GameObject* parent)
{
	object = parent;
	//make sure we have a rigidbody
	if (object->GetComponent<Rigidbody>()) {}
	else { object->AddComponent<Rigidbody>(); }

	rb = object->GetComponent<Rigidbody>();


	//change these values later these are placeholders, should be set depending on the object
	//these are defaults 
	maxAcceleration = 5;
	maxSpeed = 5;
	targetRadius = 2.0f;
	slowRadius = 4;
	timeToTarget = 0.1f;
	avoidDistance = 3.0f;
	lookAhead = 5.0f;
}    

void DynamicMovement::Update(float deltaTime)
{
	//requires target to work
	if (target)
	{


		object->GetComponent<Rigidbody>()->SetAcceleration(GetSteering());



	}
}
void DynamicMovement::SetAll(float maxAcceleration_, float maxSpeed_, float targetRadius_, float slowRadius_, float timeToTarget_)
{
	//sets all the variables at once
	maxAcceleration = maxAcceleration_; maxSpeed = maxSpeed_; targetRadius = targetRadius_; slowRadius = slowRadius_; timeToTarget = targetRadius_;
}
Vector4 DynamicMovement::GetSteering()
{
	Vector3 temp;
	Vector3 direction;
	Vector4 targetPos = target->GetPosition();
	if (shouldAvoid)
	{
		temp = Vector3(rb->GetVelocity());
	
		temp = temp.Normalize();
		
		temp = temp - rb->GetPosition();
		temp=temp.Normalize();

		Ray ray = Ray(Vector3(rb->GetPosition()), temp);
		GameObject* objectHit = CollisionHandler::GetInstance()->RayGetFirstHit(ray, nullptr);
		
		if (obstacles.find(objectHit) != obstacles.end())
		{
			//the normal is curently not working, need to get Plane to calculate the normal =(
			 temp = Vector3( objectHit->GetPosition());
			 Vector3 t = Vector3(object->GetPosition());
			t = Math::Cross(t, Vector3(objectHit->GetPosition()));
			temp=temp.Normalize();
			std::cout << "found" << std::endl;
			targetPos = (temp * avoidDistance) + objectHit->GetPosition();
		}
		
	}

	//direction=target position-object position
	if (!shouldFlee)
	{

		direction.SetX(targetPos.GetX() - object->GetPosition().GetX());
		direction.SetY(targetPos.GetY() - object->GetPosition().GetY());
		direction.SetZ(targetPos.GetZ() - object->GetPosition().GetZ());
	}
	else
	{
		direction.SetX(object->GetPosition().GetX() - targetPos.GetX());
		direction.SetY(object->GetPosition().GetY() - targetPos.GetY());
		direction.SetZ(object->GetPosition().GetZ() - targetPos.GetZ());
	}
	float distance = direction.Mag();
	
	
	//Ray ray (direction,Vector3())
	float targetSpeed;





	//stop acclerating when in range
	if (distance < targetRadius && !shouldFlee)
	{

		object->GetComponent<Rigidbody>()->SetVelocity(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
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

	direction = direction.Normalize();
	direction = direction * targetSpeed;
	temp = direction - rb->GetVelocity();
	temp = temp / timeToTarget;
	// Clip acceleration if too high
	if (temp.Mag() > maxAcceleration)
	{
		temp = temp.Normalize();
		temp = temp * maxAcceleration;
	}
	//only using x and z
	return Vector4(temp.GetX(), 0.0f, temp.GetZ(), 0.0f);
}