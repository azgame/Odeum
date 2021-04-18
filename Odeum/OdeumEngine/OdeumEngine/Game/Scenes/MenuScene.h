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
	void Unload() {};
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;
	std::vector<const char*> levelString= { "E","Planet Panic!" ,"Cake Swipers" ,  "Wool Watchers",  "Hopping aliens",  "Space Station Savants","Hopping Chickens", "Lets get this bread", "Comet Crashers", "Farm Frenzy", "A" };
	std::vector<GameObject*> gameObjects;
	void LoadLevel(int level_);
	void Quit();
	uint16_t frameCounter;
	 int i1=1;
	float averageFrameTime;
	float frameTimeTotal;


};

#endif