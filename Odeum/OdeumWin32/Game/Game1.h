#ifndef GAME1_H
#define GAME1_H

#include "../Engine/Core/OdeumEngine.h"
#include "Scenes/GameScene.h"
#include "Scenes/StartScene.h"

class Game1 : public GameInterface
{
public:
	Game1();
	virtual ~Game1();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual std::vector<GameObject*> GetRenderObjects();
private:
	int m_currentSceneNum;
	Scene* m_currentScene;
	void BuildScene();
};

#endif