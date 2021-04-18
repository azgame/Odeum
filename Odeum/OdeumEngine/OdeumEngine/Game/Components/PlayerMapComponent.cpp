#include "PlayerMapComponent.h"
#include "../../Engine/DataStructures/SimpleGraph.h"
#include "../../Engine/CoreComponents/Rigidbody.h"

void PlayerMapComponent::OnAttach(GameObject* parent)
{
	m_gameObject = parent;
	m_tileIndex = 0;
	m_startMovement = false;
	m_direction = 0;
	m_firstMove = true;
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

void PlayerMapComponent::LerpBetweenTiles(int startID, int endID, float lerpSpeed)
{
	GameObject* start = nullptr;
	GameObject* next = nullptr; 
	GameObject* end = nullptr;
	int nextID;

	start = m_map->at(startID);
	m_tileIndex = startID;
	if (m_direction == 0) {
		if((int)(startID - 1) < 0)
		{
			nextID = m_map->size() - 1;
			next = m_map->at(nextID);
		}
		else {
			nextID = startID - 1;
			next = m_map->at(nextID);
		}
	}
	else if(m_direction == 1){
		// i guess just equals would do
		if((int)(startID + 1) >= m_map->size()) {
			nextID = 0;
			next = m_map->at(nextID);
		}
		else {
			nextID = startID + 1;
			next = m_map->at(nextID);
		}
	}
	end = m_map->at(endID);
	Vector3 testVec = Vector3(Math::Lerp(m_gameObject->GetComponent<Rigidbody>()->GetPosition(), next->GetComponent<Rigidbody>()->GetPosition(), lerpSpeed));

	/*std::cout << "TESTVEC: ";
	testVec.Print();
	std::cout << std::endl;
	*/

	/*std::cout << "player mag: " << m_gameObject->GetComponent<Rigidbody>()->GetPosition().Mag() << std::endl;
	std::cout << "tile mag: " << next->GetComponent<Rigidbody>()->GetPosition().Mag() << std::endl << std::endl;
	*/
	// lerp to next tile -- use actual lerp
	m_gameObject->GetComponent<Rigidbody>()->SetPosition(Math::Lerp(m_gameObject->GetComponent<Rigidbody>()->GetPosition(), next->GetComponent<Rigidbody>()->GetPosition(), lerpSpeed));

	//if(m_gameObject->GetComponent<Rigidbody>()->GetPosition())

	//std::cout << "distacne: " << Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition() - next->GetComponent<Rigidbody>()->GetPosition()).Mag() << std::endl << std::endl;
	// if the distance between the player and next tile is less than a small number, move to next tile
	if (Vector3(m_gameObject->GetComponent<Rigidbody>()->GetPosition() - next->GetComponent<Rigidbody>()->GetPosition()).Mag() < 0.1f) {
		if (nextID == endID)
		{
			m_startMovement = false;
			m_tileIndex = endID;
			m_firstMove = false;
			return;
		}
		LerpBetweenTiles(nextID, endID, lerpSpeed);
	}
	// ignore this vv
	/*if (abs(m_gameObject->GetComponent<Rigidbody>()->GetPosition().Mag()) < abs(next->GetComponent<Rigidbody>()->GetPosition().Mag())) {
		if (abs(m_gameObject->GetComponent<Rigidbody>()->GetPosition().Mag()) >= abs(next->GetComponent<Rigidbody>()->GetPosition().Mag())) {
			// passed the tile
			// check to see if this is the last 
			if (next == end) {
				m_startMovement = false;
				m_tileIndex = m_destinationIndex;
				return;
			}
			// lerp to next tile
			else {
				LerpBetweenTiles(nextID, endID, lerpSpeed);
			}
		}		
	}
	else if (abs(m_gameObject->GetComponent<Rigidbody>()->GetPosition().Mag()) > abs(next->GetComponent<Rigidbody>()->GetPosition().Mag())) {
		if (abs(m_gameObject->GetComponent<Rigidbody>()->GetPosition().Mag()) <= abs(next->GetComponent<Rigidbody>()->GetPosition().Mag())) {
			// passed the tile
			// check to see if this is the last 
			if (next == end) {
				m_startMovement = false;
				m_tileIndex = m_destinationIndex;
				return;
			}
			// lerp to next tile
			else {
				LerpBetweenTiles(nextID, endID, lerpSpeed);
			}
		}
	}*/
}

void PlayerMapComponent::Update(float deltaTime)
{
	
	if (m_tileIndex != m_destinationIndex) {
		if (m_startMovement)
		{
			LerpBetweenTiles(m_tileIndex, m_destinationIndex, 0.05f); // could make lerpSpeed a variable
		}
	}
	else {
		m_startMovement = false;
		m_tileIndex = m_destinationIndex;
	}
}