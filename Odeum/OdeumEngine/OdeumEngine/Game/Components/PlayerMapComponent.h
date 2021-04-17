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
	void Update(float deltaTime) override;

	void AddMovement(int Movement);
	void ReduceMovement(int Movement);

	void Move(std::vector<GameObject*>* Map);

	void SetDestination(int Index) { m_destinationIndex = Index; }
	int GetDestination() { return m_destinationIndex; }
	int GetTile() { return m_tileIndex; }

	void SetStartMovement(bool m) { m_startMovement = m; }
	bool GetStartMovement() { return m_startMovement; }

	// 0 = right, 1 = left, 2 = up, 3 = down
	void SetDirection(int d) { m_direction = d; }
	// 0 = right, 1 = left, 2 = up, 3 = down
	int GetDirection() { return m_direction; } 

	void LerpBetweenTiles(int startID, int endID, float lerpSpeed);
	void AddMap(std::vector<GameObject*>* map) { m_map = map; }

private:

	UINT initiative;
	UINT m_movement;
	UINT m_tileIndex;
	UINT m_destinationIndex;
	bool m_startMovement;
	UINT m_direction; // 0 = right, 1 = left, 2 = up, 3 = down
	std::vector<GameObject*>* m_map;
};

#endif