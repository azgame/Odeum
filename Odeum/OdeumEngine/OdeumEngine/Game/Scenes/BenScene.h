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
	GameObject* object;
	GameObject* object2;
	float angle;
	float direction;
};

#endif