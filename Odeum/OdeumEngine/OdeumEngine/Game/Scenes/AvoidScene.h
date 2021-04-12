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
	float timeToEnd = 5;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	std::vector<Key::KeyCode> playerKeysDown;
	Simplex<Vector3> simplex;

	std::vector<Colour*> playerColors;

	float maxVelocity = 7;
	float maxHeight = 20;
	float minHeight = -20;
	float minRight = -30;
	float maxRight = 20;
	int maxScore = 10;
	bool won;
};

#endif