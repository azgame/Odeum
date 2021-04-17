#ifndef ANIMATIONTESTSCENE_H
#define ANIMATIONTESTSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"

class ParticleEffect;

// Scene index 1
class AnimationTestScene : public Scene
{
public:
	AnimationTestScene();
	virtual ~AnimationTestScene();

	virtual bool Initialize();
	void Unload() {};
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

private:
	CameraController cameraController;

	//std::vector<GameObject*> gameObjects;

	GameObject* go1;
	GameObject* go2;

	ParticleEffect* particle;

	uint16_t frameCounter;
	float averageFrameTime;
	float frameTimeTotal;

};

#endif