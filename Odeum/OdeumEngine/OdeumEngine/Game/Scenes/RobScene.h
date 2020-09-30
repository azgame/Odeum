#ifndef ROBSCENE_H
#define ROBSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 1
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
	GameObject* plane;
	GameObject* newObject;
};

#endif