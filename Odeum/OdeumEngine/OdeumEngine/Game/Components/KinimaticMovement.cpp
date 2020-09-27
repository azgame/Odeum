#include "KinimaticMovement.h"
void KinimaticMovement::Update(float deltaTime)
{
	//object->SetPosition(object->GetPosition() + Vector4(0.01f, 0.0f, 0.0f, 0.0f));
	// Component does stuff
	if (target)
	{
		Debug::Info("target aquired", __FILENAME__, __LINE__);
	}
}
void KinimaticMovement::OnAttach(GameObject* parent)
{
	object = parent;
}
Vector4 KinimaticMovement::GetSteering()
{
	Vector4 velocity(0.0f, 0.0f, 0.0f, 0.0f);
	
	float rotation;
	velocity = target->GetPosition() - object->GetPosition();
	
	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}
