#ifndef COLLECTINGSCENE_H
#define COLLECTINGSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class CollectingScene : public Scene
{
public:
	CollectingScene();
	virtual ~CollectingScene();
	int MaxPlayers;
	int MaxTargets=6;
	//float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	
	virtual void UIRender();
	void UpdateScore(int scorePosition_, int score_);
	void spawnObjects(float deltaTime_);
private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	std::vector<GameObject*> targetObjects;
	std::vector<float> objectSpawnTimes;
	std::vector<bool> objectSpawned;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	
	//these ensure the player may not hold the button
	
	std::vector<float> playerScores;
	//targetHeights is the three difrent heights the player needs to maintain.
	float maxVelocity=7;
	float maxHeight=9;
	float minHeight=-9;
	float minRight=-17.5;
	float maxRight=17.5;
	int maxScore=10;
	bool won;
};

#endif