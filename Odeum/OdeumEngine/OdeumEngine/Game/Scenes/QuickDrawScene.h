#ifndef QUICKDRAWSCENE_H
#define QUICKDRAWSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

// Scene index 7
class QuickDrawScene : public Scene
{
public:
	QuickDrawScene();
	virtual ~QuickDrawScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	std::vector<GameObject*> players;
	std::vector<float> playerShootTimer;
	std::vector<int> playerScore;
	int maxScore;
	int maxPlayers;
	bool targetHit ;
	float timeUntilTarget;
	std::vector<Key::KeyCode> playerKeys;
	
};

#endif