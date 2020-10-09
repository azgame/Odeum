#ifndef TESTRENDER_H
#define TESTRENDER_H

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/CommandContext.h"
#include "../Rendering/DirectX12/Model.h"
#include "Camera.h"
#include "CoreSystem.h"

class TestRender : public CoreSystem
{
public:

	TestRender();
	~TestRender();

	void Attach() override;
	void Detach() override;
	void Update(float deltaTime_) override;
	void UIRender() override;
	void HandleEvent(Event& event_) override;

private:

	RootSignature m_rootSig;
	GraphicsPSO m_colourPSO;
	D3D12_VIEWPORT m_mainViewport;
	D3D12_RECT m_mainScissor;
	ID3D12DescriptorHeap* m_pHeap;

	void CreateUIResources();
	void InitializeUI();
};

#endif