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
	targetRadius = 0.0f;
	slowRadius = 4;
	timeToTarget = 0.1f;
	avoidDistance = 2.3f;

}

void DynamicMovement::Update(float deltaTime)
{
	//requires target to work
	if (target)
	{


		object->GetComponent<Rigidbody>()->SetAcceleration(GetSteering());

		//object->GetComponent<Rigidbody>()->SetAcceleration(Vector4(0,0,0,0));

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


		Ray forward = Ray(Vector3(object->GetPosition()), Vector3(rb->GetVelocity().Normalize()));
		Vector4 intersectionPlane;

		// Get a list of collided objects
		std::vector<GameObject*> colliders = CollisionHandler::GetInstance()->RayGetList(forward);

		// get the intersection plane from any of the objects that were collided with
		for (auto obj : colliders)
		{
			if (obj->Tag() == "Obstacle")
				intersectionPlane = CollisionDetection::RayOBBIntersectionPlane(forward, obj->GetBoundingBox());
		}
		
		// Get the first collided object
		// If you pass in a Vector4*, it'll be filled with the intersection plane where the ray collided with the obb, if there was a collision
		GameObject* newtarget = CollisionHandler::GetInstance()->RayGetFirstHit(forward, &intersectionPlane);
		if(newtarget!=NULL)
		std::cout << newtarget->Tag()<< std::endl;
		if (newtarget!=NULL&&newtarget->Tag()== "Obstacle")
		{
			Vector3 normal = Vector3(intersectionPlane); // the normal is the first three components of the plane
			std::cout << intersectionPlane.ToString() << std::endl;
			//the normal is curently not working, need to get Plane to calculate the normal =(
			//target = collision.position
			//+collision.normal * avoidDistance
			//	return Seek.getSteering()
			temp = Vector3(newtarget->GetPosition() + normal * avoidDistance);
			//std::cout << newtarget->GetPosition().ToString() << " " << normal.ToString() << std::endl;
			//std::cout << "found" << std::endl;
			targetPos = Vector4(temp);
			//(5.000000, 0.000000, 0.000000, 1.000000) (-1.000000, 0.000000, 0.000000)
		}
		else
		{
			std::cout << "not found" << std::endl;
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