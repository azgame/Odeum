#ifndef SPHEREPUSHINGSCENE_H
#define SPHEREPUSHINGSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class SpherePushingScene : public Scene
{
public:
	SpherePushingScene();
	virtual ~SpherePushingScene();
	int MaxPlayers;
	
	//float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);

	virtual void UIRender();
	
private:
	//each of the three objects for each player
	std::vector<GameObject*> playerObjects;
	


	std::vector<GameObject*> backgroundObjects;
	//change these to players later
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	std::vector<Key::KeyCode> playerKeysDown;
	std::vector<Key::KeyCode> playerKeysJump;
	Simplex<Vector3> simplex;



	float maxVelocity = 7;
	float maxHeight = 9;
	float minHeight = -9;
	float minRight = -17.5;
	float maxRight = 17.5;
	float minUp = -10;
	float maxUp = 0;
	int maxScore = 10;
	bool won;
};

#endif