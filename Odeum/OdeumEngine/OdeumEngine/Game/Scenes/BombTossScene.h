#ifndef BOMBTOSSSCENE_H
#define BOMBTOSSSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 6
class BombTossScene : public Scene
{
public:
	BombTossScene();
	virtual ~BombTossScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> playerObjects;

	int bombTimer = 100;
	bool gameOver = false;
};

#endif#pragma once
