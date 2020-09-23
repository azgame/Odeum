#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 0
class StartScene : public Scene
{
public:
	StartScene();
	virtual ~StartScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();
private:
};

#endif