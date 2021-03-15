#ifndef DODGESCENE_H
#define DODGESCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class DodgeScene : public Scene
{
public:
	DodgeScene();
	virtual ~DodgeScene();
	int MaxPlayers;
	int MaxObstacles = 8;
	//float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);

	virtual void UIRender();
	void spawnObjects(float deltaTime_);
private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	std::vector<GameObject*> obstacleObjects;
	std::vector<float> objectSpawnTimes;
	std::vector<bool> objectSpawned;
	std::vector<bool> playerDead;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	Simplex<Vector3> simplex;
	//these ensure the player may not hold the button

	
	//targetHeights is the three difrent heights the player needs to maintain.
	float maxVelocity = 7;
	float maxHeight = 9;
	float minHeight = -9;
	float minRight = -17.5;
	float maxRight = 17.5;
	int currentDeadPlayers;
	bool won;
};

#endif