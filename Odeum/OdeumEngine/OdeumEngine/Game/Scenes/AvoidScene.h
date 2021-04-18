#ifndef AVOIDSCENE_H
#define AVOIDSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

#include "../../Engine/Core/CameraController.h"

class AvoidScene : public Scene
{
public:
	AvoidScene();
	virtual ~AvoidScene();

	//float timeToStart;
	virtual bool Initialize();
	void Unload() {};
	virtual void Update(const float deltaTime_);

	virtual void UIRender();

private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	std::vector<bool> playerDead;
	std::vector<Vector4> playerLastVelocity;
	std::vector<GameObject*> backgroundObjects;
	std::vector<GameObject*> obstacleObjects;
	GameObject* enemyObject;

	int MaxPlayers;
	int currentDeadPlayers;
	float timeToWin = 30.0f;
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

	const int kMaxObstacles = 5;
};

#endif