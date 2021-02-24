#ifndef MULTITASKINGSCENE_H
#define MULTITASKINGSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class MultiTaskingScene : public Scene
{
public:
	MultiTaskingScene();
	virtual ~MultiTaskingScene();
	int MaxPlayers;
	float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	void UpdateObjectsB(const float deltaTime_);
	void UpdateObjectsC(const float deltaTime_);
	virtual void UIRender();

private:
	//each of the three objects for each player
	std::vector<GameObject*> gameObjectsA;
	std::vector<GameObject*> gameObjectsB;
	std::vector<GameObject*> gameObjectsC;
	std::vector<GameObject*> targetObjects;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysA;
	std::vector<Key::KeyCode> playerKeysB;
	std::vector<Key::KeyCode> playerKeysC;
	//these ensure the player may not hold the button
	std::vector<bool> playerPressedA;
	std::vector<bool> playerPressedB;
	std::vector<bool> playerPressedC;
	std::vector<float> playerScores;
	//targetHeights is the three difrent heights the player needs to maintain.
	std::vector<float> targetHeights;
	bool won;
};

#endif