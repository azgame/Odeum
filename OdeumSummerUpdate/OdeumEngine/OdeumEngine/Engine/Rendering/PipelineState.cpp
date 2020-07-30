#include "PipelineState.h"

#include "RootSignature.h"

#include "D3DCore.h"

void PSO::Destroy()
{
	m_rootSignature = nullptr;
}

GraphicsPSO::GraphicsPSO()
{
	ZeroMemory(&m_psoDesc, sizeof(m_psoDesc));
	m_psoDesc.NodeMask = 1;
	m_psoDesc.SampleMask = 0xFFFFFFFFu;
	m_psoDesc.SampleDesc.Count = 1;
	m_psoDesc.InputLayout.NumElements = 0;
}

void GraphicsPSO::SetBlendState(const D3D12_BLEND_DESC& blendDesc_)
{
	m_psoDesc.BlendState = blendDesc_;
}

void GraphicsPSO::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc_)
{
	m_psoDesc.RasterizerState = rasterizerDesc_;
}

void GraphicsPSO::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc_)
{
	m_psoDesc.DepthStencilState = depthStencilDesc_;
}

void GraphicsPSO::SetSampleMask(UINT sampleMask_)
{
	m_psoDesc.SampleMask = sampleMask_;
}

void GraphicsPSO::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_)
{
	assert(topologyType_ != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED);
	m_psoDesc.PrimitiveTopologyType = topologyType_;
}

void GraphicsPSO::SetRenderTargetFormat(DXGI_FORMAT rtvFormat_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_, UINT MsaaQuality_)
{
	SetRenderTargetFormats(1, &rtvFormat_, dsvFormat_, MsaaCount_, MsaaQuality_);
}

void GraphicsPSO::SetRenderTargetFormats(UINT numRTVs_, const DXGI_FORMAT* rtvFormats_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_, UINT MsaaQuality_)
{
	assert(numRTVs_ == 0 || rtvFormats_ != nullptr);

	for (UINT i = 0; i < numRTVs_; i++)
		m_psoDesc.RTVFormats[i] = rtvFormats_[i];

	for (UINT i = numRTVs_; i < m_psoDesc.NumRenderTargets; i++)
		m_psoDesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;

	m_psoDesc.NumRenderTargets = numRTVs_;
	m_psoDesc.DSVFormat = dsvFormat_;
	m_psoDesc.SampleDesc.Count = MsaaCount_;
	m_psoDesc.SampleDesc.Quality = MsaaQuality_;
}

void GraphicsPSO::SetInputLayout(UINT numElements_, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs_)
{
	m_psoDesc.InputLayout.NumElements = numElements_;

	if (numElements_ > 0)
	{
		D3D12_INPUT_ELEMENT_DESC* elements = (D3D12_INPUT_ELEMENT_DESC*)malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements_);
		memcpy(elements, inputElementDescs_, sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements_);
		m_inputLayouts.reset((const D3D12_INPUT_ELEMENT_DESC*)elements);
	}
	else
		m_inputLayouts = nullptr;
}

void GraphicsPSO::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE indexBufferProps_)
{
	m_psoDesc.IBStripCutValue = indexBufferProps_;
}

void GraphicsPSO::Finalize()
{
	m_psoDesc.pRootSignature = m_rootSignature->GetRootSignature();
	assert(m_psoDesc.pRootSignature != nullptr);

	m_psoDesc.InputLayout.pInputElementDescs = m_inputLayouts.get();

	if (FAILED(DXGraphics::m_device->CreateGraphicsPipelineState(&m_psoDesc, IID_PPV_ARGS(&m_pso))))
		Debug::FatalError("Graphics PSO could not be created!", __FILENAME__, __LINE__);
}
