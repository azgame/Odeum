#ifndef BOMBTOSS_H
#define BOMBTOSS_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 9
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
};

#endif#pragma once
