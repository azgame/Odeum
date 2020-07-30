#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"

class StartScene : public Scene
{
public:
	StartScene();
	virtual ~StartScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual std::vector<GameObject*>* GetRenderObjects() { return &m_renderObjects; }
private:
	std::vector<GameObject*> m_renderObjects;
};

#endif