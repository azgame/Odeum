#ifndef _ODEUMENGINE_H
#define _ODEUMENGINE_H

#include "../../pch.h"

#include "Input.h"
#include "../Rendering/Renderer.h"
#include "Camera.h"
#include "Timer.h"
#include "GameInterface.h"
#include "Scene.h"

#include <memory>

class OdeumEngine
{
public:
	OdeumEngine(const OdeumEngine&) = delete;
	OdeumEngine(OdeumEngine&&) = delete;
	OdeumEngine& operator=(const OdeumEngine&) = delete;
	OdeumEngine& operator=(OdeumEngine&&) = delete;

	bool Initialize(int, int, HWND);
	bool Run();
	void Uninitialize();

	static OdeumEngine* GetInstance();

	void SetGameInterface(GameInterface* gameInterface_);
	int GetCurrentScene();
	void SetCurrentScene(int sceneNum_);

	void Shutdown();
private:
	OdeumEngine();
	~OdeumEngine();

	static std::unique_ptr<OdeumEngine> engineInstance;
	friend std::default_delete<OdeumEngine>;

	Renderer* m_renderer;
	Camera* m_mainCamera;
	std::vector<GameObject*> m_renderObjects;
	float m_pitch, m_yaw;

	Timer timer;
	unsigned int fps;

	int m_currentSceneNum;

	GameInterface* m_gameInterface;
};

#endif // !_ODEUMMAIN_H