#ifndef BUTTONMASHINGSCENE_H
#define BUTTONMASHINGSCENE_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include"../../Engine/Core/AudioHandler.h"
#include "../../Engine/Rendering/DirectX12/ParticleManager.h"


// Scene index 8
class ButtonMashingScene : public Scene
{
public:
	ButtonMashingScene();
	virtual ~ButtonMashingScene();
	int MaxPlayers;
	float timeToStart;
	virtual bool Initialize();
	void Unload();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();


private:

	Vector4 clear_color=Vector4(0,0,0,0);
	CameraController cameraController;
	std::vector<Colour> playerColors;
	float timeToEnd = 5;
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> backgroundObjects;
	std::vector<GameObject*> moonObjects;
	std::vector<Key::KeyCode> playerKeys;
	std::vector<bool> playerPressed;
	bool won;

	std::vector<ParticleEffect*> pEffects;
	ParticleProperties rocketProps;
	ParticleProperties explosionProps;
};

#endif