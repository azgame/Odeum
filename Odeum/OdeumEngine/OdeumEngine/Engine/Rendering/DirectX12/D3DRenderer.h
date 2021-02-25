#ifndef D3DRENDERER_H
#define D3DRENDERER_H

#include "../Common/Renderer.h"
#include "CommandContext.h"
#include "Model.h"

#include "../../Core/Camera.h"

class D3DRenderer : public AbstractRenderer
{
public:

	D3DRenderer() {}

	void Initialize(Window& Window) override;
	void Update(float deltaTime_) override;
	virtual void Render(Camera& Camera, float deltaTime) override;
	void UIRender() override;
	virtual void Uninitialize() override;

private:

	Camera mainCamera;

	RootSignature m_rootSig;
	GraphicsPSO m_colourPSO;
	D3D12_VIEWPORT m_mainViewport;
	D3D12_RECT m_mainScissor;
	ID3D12DescriptorHeap* m_pHeap;

	void CreateUIResources();
	void InitializeUI(Window& Window);
	void UIRenderD3DResources();

	uint16_t frameCounter;
	float averageFrameTime;
	float frameTimeTotal;

	uint32_t particleCount;
	float averageNumParticles;
	float totalNumParticles;
};

#endif