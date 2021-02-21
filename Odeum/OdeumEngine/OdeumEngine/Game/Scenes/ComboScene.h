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
	std::vector<GameObject*> playerObjects;
	std::vector<int> combo;

	// need to fix player component to easily have these in it.
	// for now each player's keys are hard coded into these vectors.
	std::vector<Key::KeyCode> playerKeysLeft;
	std::vector<Key::KeyCode> playerKeysRight;
	std::vector<Key::KeyCode> playerKeysUp;
	std::vector<Key::KeyCode> playerKeysDown;
};

#endif