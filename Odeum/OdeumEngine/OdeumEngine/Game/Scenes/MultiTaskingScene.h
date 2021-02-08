#ifndef MULTITASKING_H
#define MULTITASKING_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class MultiTaskingScene : public Scene
{
public:
	MultiTaskingScene();
	virtual ~MultiTaskingScene();
	int MaxPlayers;
	float timeToStart;
	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	void UpdateObjectsB();
	void UpdateObjectsC();
	virtual void UIRender();

private:
	std::vector<GameObject*> gameObjectsA;
	std::vector<GameObject*> gameObjectsB;
	std::vector<GameObject*> gameObjectsC;
	std::vector<Key::KeyCode> playerKeysA;
	std::vector<Key::KeyCode> playerKeysB;
	std::vector<Key::KeyCode> playerKeysC;
	std::vector<bool> playerPressedA;
	std::vector<bool> playerPressedB;
	std::vector<bool> playerPressedC;
	bool won;
};

#endif