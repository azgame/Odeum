#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Core/OdeumEngine.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();

};

#endif