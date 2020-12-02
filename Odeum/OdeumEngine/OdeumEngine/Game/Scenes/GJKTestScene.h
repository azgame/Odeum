#ifndef GJKTESTSCENE_H
#define GJKTESTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../Engine/Math/D3DMath.h"
#include "../Engine/Math/Collider.h"
#include "../Engine/Math/CollisionDetection.h"

class GJKTestScene : public Scene
{
public:
	GJKTestScene();
	virtual ~GJKTestScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;
	Collider* c1;
	Collider* c2;

};

#endif