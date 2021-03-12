#ifndef MAP_H
#define MAP_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../../Engine/DataStructures/Graph.h"

#include "../GameObjects/Action.h"

class MAPScene : public Scene
{
public:
	MAPScene();
	virtual ~MAPScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	Graph<GameObject*> map = Graph<GameObject*>(100, false);

	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> playerObjects;
	
	CameraController cameraController;
	float angle;
	float direction;
};
#endif