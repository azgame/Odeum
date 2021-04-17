#ifndef CAMSCENE_H
#define CAMSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"


// Scene index 1
class CamScene : public Scene
{
public:
	CamScene();
	virtual ~CamScene();
	std::vector<GameObject*> gameObjects;
	virtual bool Initialize();
	void Unload() {};
	virtual void Update(const float deltaTime_);
	virtual void UIRender();
	Vector3 Pos;
private:
	CameraController cameraController;

	
	float angle;
	float direction;

	
};

#endif
