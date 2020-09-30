#ifndef BENSCENE_H
#define BENSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 1
class BenScene : public Scene
{
public:
	BenScene();
	virtual ~BenScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();

private:
	GameObject* object;
	GameObject* plane;
	GameObject* newObject;
};

#endif