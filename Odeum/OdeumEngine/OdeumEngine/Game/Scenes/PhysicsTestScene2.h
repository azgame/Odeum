#ifndef PHYSICSTESTSCENE2_H
#define PHYSICSTESTSCENE2_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include "../Engine/Math/D3DMath.h"
#include "../Engine/Math/Collider.h"
#include "../Engine/Math/CollisionDetection.h"

class PhysicsTestScene2 : public Scene
{
public:
	PhysicsTestScene2();
	virtual ~PhysicsTestScene2();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

	void InitObjects();
	void CollisionResponse();

private:
	CameraController cameraController;

	GameObject* go_a;
	GameObject* go_s;

	bool isMoving;
	bool isRotating;
	bool isRotationalVelocity;

	float rotationCounter;

};
#endif