#ifndef _ODEUMENGINE_H
#define _ODEUMENGINE_H

#include "../../pch.h"

#include "Input.h"
#include "../Rendering/Renderer.h"
#include "Camera.h"

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

private:
	OdeumEngine();
	~OdeumEngine();

	static std::unique_ptr<OdeumEngine> engineInstance;
	friend std::default_delete<OdeumEngine>;

	Input* m_input = Input::getInstance();
	Renderer* m_renderer;
	Camera* m_mainCamera;
	std::vector<Model*> m_renderObjects;
	float m_pitch, m_yaw;

	void UpdateCamera();
};

#endif // !_ODEUMMAIN_H