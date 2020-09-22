#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();

private:
	GameObject* object;
	GameObject* plane;
	GameObject* newObject;
};

#endif