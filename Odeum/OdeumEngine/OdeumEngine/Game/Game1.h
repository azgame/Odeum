#ifndef GAME1_H
#define GAME1_H

#include "../pch.h"

#include "../Engine/Core/OdeumEngine.h"

#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"
#include "Scenes/RobScene.h"
#include "Scenes/BenScene.h"
#include "Scenes/BenScene2.h"

class Game1 : public GameInterface
{
public:
	Game1();
	virtual ~Game1();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();
private:
	int								m_currentSceneNum;
	Scene*							m_currentScene;
	void BuildScene();
};

#endif