#ifndef COMBOSCENE_H
#define COMBOSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class ComboScene : public Scene {
public:
	ComboScene();
	virtual ~ComboScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjects;

	std::vector<int> combo;
};

#endif