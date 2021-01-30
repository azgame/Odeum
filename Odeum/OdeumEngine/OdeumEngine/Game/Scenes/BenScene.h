#ifndef BENSCENE_H
#define BENSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"

// Scene index 1
class BenScene : public Scene
{
public:
	BenScene();
	virtual ~BenScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;

	//std::vector<GameObject*> gameObjects;

	GameObject* go1;
	GameObject* go2;
	float angle;
	float direction;

	uint16_t frameCounter;
	float averageFrameTime;
	float frameTimeTotal;

	Simplex<Vector3> simplex;

	float t;
};

#endif