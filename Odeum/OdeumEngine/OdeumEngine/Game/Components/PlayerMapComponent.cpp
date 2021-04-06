#include "PlayerMapComponent.h"
#include "../../Engine/DataStructures/SimpleGraph.h"
#include "../../Engine/CoreComponents/Rigidbody.h"

void PlayerMapComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
}

void PlayerMapComponent::OnStart()
{
}

void PlayerMapComponent::AddMovement(int Movement)
{
	// temp
	m_movement = Movement;
}

// Reduces total number of tiles this unit can move by Param1
void PlayerMapComponent::ReduceMovement(int Movement)
{
	m_movement -= Movement;
}

void PlayerMapComponent::Move(std::vector<GameObject*>* Map)
{
	GameObject* target = Map->at(m_destinationIndex);

	m_gameObject->GetComponent<Rigidbody>()->SetPosition(Vector4(
		target->GetComponent<Rigidbody>()->GetPosition().GetX(), 
		target->GetComponent<Rigidbody>()->GetPosition().GetY() + 1,
		target->GetComponent<Rigidbody>()->GetPosition().GetZ(),
		1));

	m_tileIndex = m_destinationIndex;
}
