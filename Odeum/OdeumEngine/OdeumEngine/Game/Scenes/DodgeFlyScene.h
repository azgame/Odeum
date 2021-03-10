#ifndef DODGEFLYSCENE_H
#define DODGEFLYSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class DodgeFlyScene : public Scene
{
public:
	DodgeFlyScene();
	virtual ~DodgeFlyScene();
	int MaxPlayers;
	int MaxObstaclesLeft = 10;
	int MaxObstaclesRight = 10;
	int MaxObstaclesUp = 20;
	//float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);

	virtual void UIRender();
	void spawnObjects(float deltaTime_);
private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	std::vector<GameObject*> obstacleObjects;
	//lane 1 = left 
	//lane 2 = right 
	//lane 3 = up
	std::vector<int> obstacleLane;
	std::vector<GameObject*> backgroundObjects;
	std::vector<float> objectSpawnTimes;
	std::vector<bool> objectSpawned;
	std::vector<bool> playerDead;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	std::vector<Key::KeyCode> playerKeysDown;
	std::vector<Key::KeyCode> playerKeysJump;
	Simplex<Vector3> simplex;
	//these ensure the player may not hold the button


	//targetHeights is the three difrent heights the player needs to maintain.
	float maxVelocity = 7;
	float maxHeight = 9;
	float minHeight = -9;
	float minRight = -17.5;
	float maxRight = 17.5;
	float minUp = -10;
	float maxUp = 0;
	int currentDeadPlayers=0;
	bool won;
};

#endif