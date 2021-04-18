#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../../Engine/DataStructures/Graph.h"
#include "../../Engine/DataStructures/SimpleGraph.h"
#include "../../Engine/DataStructures/PriorityQueue.h"

#include "../GameObjects/Action.h"

#include <stdlib.h>

class PlayerMapComponent;
class CameraController;

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
		roll = die.Roll();
		GameRollEvent gameRoll = GameRollEvent(target, roll);
	};

	int GetRoll() { return roll; }

	GameEventType Type() override { return GameEventType::GameRoll; }
private:
	GameObject* target;
	Dice die;
	int roll;
};

class MAPScene : public Scene
{
public:
	MAPScene();
	virtual ~MAPScene();

	virtual bool Initialize();
	void Unload() {};
	virtual void Update(const float deltaTime_);
	virtual void UIRender();
private:

	bool PlayerTurn();

	void LoadLevel(int level_);

	Graph<GameObject*> map = Graph<GameObject*>(100, false);

	SimpleGraph<PlayerMapComponent*>* m_map;
	std::queue<PlayerMapComponent*> players;
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> pathObjects;
	std::vector<GameObject*> playerObjects;
	std::vector<GameObject*> backgroundObjects;
	//length will be the number of tiles, vertical will make the path vertical rather than horizontal
	void SetPath(int length_, bool vertical_, Vector4 startPos_);
	void SetArea(int length_,int width, Vector4 startPos_);
	void BuildEnviroment();
	CameraController camera;

	PlayerMapComponent* m_currentPlayer;
	int numMoves = 0;
	int dieRoll = 0;
	std::string tile;
	bool hasRolled = false;
};

#endif