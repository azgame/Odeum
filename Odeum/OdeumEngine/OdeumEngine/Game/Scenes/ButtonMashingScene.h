#ifndef BUTTONMASHINGSCENE_H
#define BUTTONMASHINGSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 8
class ButtonMashingScene : public Scene
{
public:
	ButtonMashingScene();
	virtual ~ButtonMashingScene();
	int MaxPlayers;
	float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjects;
	std::vector<Key::KeyCode> playerKeys;
	std::vector<bool> playerPressed;
	bool won;
};

#endif