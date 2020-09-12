#ifndef TESTRENDER_H
#define TESTRENDER_H

#include "../Rendering/DirectX12/D3DCore.h"
#include "../Rendering/DirectX12/DXCommandContext.h"
#include "Model.h"

class TestRender
{
public:

	TestRender();
	~TestRender();

	void Initialize();
	void Render();

private:

	Model m_model;
	RootSignature m_rootSig;
	GraphicsPSO m_colourPSO;
	D3D12_VIEWPORT m_mainViewport;
	D3D12_RECT m_mainScissor;
};

#endif