#ifndef ROBSCENE_H
#define ROBSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 2
class RobScene : public Scene
{
public:
	RobScene();
	virtual ~RobScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();

private:
	float teletime;
	GameObject* object;
	
	GameObject* newObject;
};

#endif