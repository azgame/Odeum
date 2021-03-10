#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"

#include "../GameObjects/Action.h"

// Scene index 1
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;

	std::vector<GameObject*> gameObjects;
	float angle;
	float direction;

	uint16_t frameCounter;
	float averageFrameTime;
	float frameTimeTotal;

	Item* helmet;
	Item* chest;
};

#endif