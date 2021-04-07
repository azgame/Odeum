#ifndef COUNTING_H
#define OUNTING_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"


class Counting : public Scene
{
public:
	Counting();
	virtual ~Counting();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> players;
	std::vector<Key::KeyCode> playerKeys;
	std::vector<bool> playerPressed;
	std::vector<bool> playerWon;
	std::vector<int> playerScore;
	std::vector<int> playerCount;
	void CalculateWinner();
	float timeToSpawn;
	float timeToWin;
	bool won;
	
	int maxCount;
	int count;
	int MaxPlayers;
};

#endif