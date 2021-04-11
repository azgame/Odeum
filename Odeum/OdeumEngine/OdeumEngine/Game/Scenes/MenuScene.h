#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../Engine/Core/OdeumEngine.h"
#include "../GameObjects/Action.h"

// Scene index 
class MenuScene : public Scene
{
public:
	MenuScene();
	virtual ~MenuScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;
	std::vector<const char*> levelString= { "E","Planet Panic!" ,"Cake Swipers" ,  "Level B",  "Test 3",  "Test D hehe","Level 5", "Test 6", "who did this xDDDDD", "Level 8", "A" };
	std::vector<GameObject*> gameObjects;
	void LoadLevel(int level_);
	uint16_t frameCounter;
	 int i1;
	float averageFrameTime;
	float frameTimeTotal;


};

#endif