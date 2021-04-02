#ifndef AVOIDSCENE_H
#define AVOIDSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class AvoidScene : public Scene
{
public:
	AvoidScene();
	virtual ~AvoidScene();
	int MaxPlayers;
	int MaxObstacles=1;

	//float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);

	virtual void UIRender();

private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	std::vector<bool> playerDead;
	std::vector<Vector4> playerLastVelocity;
	std::vector<GameObject*> backgroundObjects;
	std::vector<GameObject*> obstacleObjects;
	int currentDeadPlayers;
	float timeToWin=30.0f;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	std::vector<Key::KeyCode> playerKeysDown;
	Simplex<Vector3> simplex;



	float maxVelocity = 7;
	float maxHeight = 9;
	float minHeight = -9;
	float minRight = -17.5;
	float maxRight = 17.5;
	int maxScore = 10;
	bool won;
};

#endif