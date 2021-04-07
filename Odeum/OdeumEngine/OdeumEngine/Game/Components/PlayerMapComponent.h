#ifndef PLAYERMAPCOMPONENT_H
#define PLAYERMAPCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

template <typename T>
class SimpleGraph;

class PlayerMapComponent : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void OnStart() override;
	void Update(float deltaTime) {};

	void AddMovement(int Movement);
	void ReduceMovement(int Movement);

	void Move(std::vector<GameObject*>* Map);

	void SetDestination(int Index) { m_destinationIndex = Index; }
	int GetDestination() { return m_destinationIndex; }
	int GetTile() { return m_tileIndex; }

private:

	UINT initiative;
	UINT m_movement;
	UINT m_tileIndex;
	UINT m_destinationIndex;
};

#endif