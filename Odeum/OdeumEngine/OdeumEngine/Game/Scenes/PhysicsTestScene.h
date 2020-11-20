#ifndef PHYSICSTESTSCENE_H
#define PHYSICSTESTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../Engine/Math/D3DMath.h"
#include "../Engine/Math/Collider.h"
#include "../Engine/Math/CollisionDetection.h"

class PhysicsTestScene : public Scene
{
public:
	PhysicsTestScene();
	virtual ~PhysicsTestScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;
	Collider* c_1;
	Collider* c_2;

	GameObject* go_1;
	GameObject* go_2;

	bool isMoving;
	bool isRotating;
	bool isRotationalVelocity;

	float rotationCounter;

};
#endif