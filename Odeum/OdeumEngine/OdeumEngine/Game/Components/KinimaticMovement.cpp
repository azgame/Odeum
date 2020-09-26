#include "KinimaticMovement.h"
void KinimaticMovement::Update(float deltaTime)
{
	object->SetPosition(object->GetPosition() + Vector4(0.01f, 0.0f, 0.0f, 0.0f));
	// Component does stuff
	
}
void KinimaticMovement::OnAttach(GameObject* parent) 
{
	object = parent;
}