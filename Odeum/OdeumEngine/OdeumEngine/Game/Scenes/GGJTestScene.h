#ifndef GGJTESTSCENE_H
#define GGJTESTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 7
class GGJTestScene : public Scene
{
public:
	GGJTestScene();
	virtual ~GGJTestScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjects;
};

#endif