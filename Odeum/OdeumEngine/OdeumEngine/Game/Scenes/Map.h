#ifndef MAP_H
#define MAP_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../../Engine/DataStructures/Graph.h"

#include "../GameObjects/Action.h"

#include <stdlib.h>

class Dice
{
public:

	Dice(int Size) { size = Size; }

	int Roll()
	{
		return rand() % (size - 1) + 1;
	}

private:
	int size;
};

class RollAction : public Action
{
public:
	RollAction(GameObject* Target, Dice Die) : die(1) { target = Target; die = Die; }

	void Execute() override
	{
		int roll = die.Roll();
		GameRollEvent gameRoll = GameRollEvent(target, roll);
	};

	GameEventType Type() override { return GameEventType::GameRoll; }
private:
	GameObject* target;
	Dice die;
};

class MAPScene : public Scene
{
public:
	MAPScene();
	virtual ~MAPScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();
	
	int RollDice(int size, int numDie);

private:
	Graph<GameObject*> map = Graph<GameObject*>(100, false);

	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> playerObjects;
	

	int currPlayerTileIndex = 0;
	int currTileIndex = 0;
	int turn = 0;
	int playerTurn = 1;
	int numMoves = 0;
	CameraController cameraController;
	float angle;
	float direction;


};
#endif