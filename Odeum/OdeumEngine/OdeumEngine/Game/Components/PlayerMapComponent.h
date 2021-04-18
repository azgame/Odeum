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

	void SetStartingOrientation(Quaternion start) { m_startingOrientation = start; }

	void SetLerpSpeed(float s) { lerpSpeed = s; }
	void SetSlerpSpeed(float s) { slerpSpeed = s; }

	bool IsFirstMove() { return m_firstMove; }
	void SetFirstMove(bool firstMove) { m_firstMove = firstMove; }
	// 0 = 'D' (backward along the gameObjects vector), 1 = 'A' (forward along the gameObjects vector)
	void SetDirection(int d) { m_direction = d; }
	// 0 = 'D' (backward along the gameObjects vector), 1 = 'A' (forward along the gameObjects vector)
	int GetDirection() { return m_direction; }

	void LerpBetweenTiles(int startID, int endID, float lerpSpeed, float slerpSpeed);
	void AddMap(std::vector<GameObject*>* map) { m_map = map; }

private:

	UINT initiative;
	UINT m_movement;
	UINT m_tileIndex;
	UINT m_destinationIndex;
	bool m_startMovement;
	bool m_firstMove;
	Quaternion m_startingOrientation;
	UINT m_direction; // 0 = 'D' (backward along the gameObjects vector), 1 = 'A' (forward along the gameObjects vector)
	std::vector<GameObject*>* m_map;
	float slerpSpeed;
	float lerpSpeed;
};

#endif