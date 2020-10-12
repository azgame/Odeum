#include "PipelineState.h"

#include "RootSignature.h"
#include "D3DCore.h"

#ifdef _MSC_VER
#pragma comment(lib, "d3dcompiler") // Automatically link with d3dcompiler.lib as we are using D3DCompile() below.
#endif

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
	ASSERT(topologyType_ != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, "We don't support undefined topology in the graphics pso");
	m_psoDesc.PrimitiveTopologyType = topologyType_;
}

void GraphicsPSO::SetRenderTargetFormat(DXGI_FORMAT rtvFormat_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_, UINT MsaaQuality_)
{
	SetRenderTargetFormats(1, &rtvFormat_, dsvFormat_, MsaaCount_, MsaaQuality_);
}

void GraphicsPSO::SetRenderTargetFormats(UINT numRTVs_, const DXGI_FORMAT* rtvFormats_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_, UINT MsaaQuality_)
{
	ASSERT(numRTVs_ == 0 || rtvFormats_ != nullptr, "Either the number of rtvs must be 0 and no format is set, or the format is given and we have more than 0 rtvs");

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

void GraphicsPSO::CompileVertexShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_)
{
	ID3DBlob* vertexShader;

	if (FAILED(D3DCompileFromFile(file_, NULL, NULL, entryPoint_, targetProfile_, D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &vertexShader, NULL)))
		Debug::Error("Could not create vertex shader", __FILENAME__, __LINE__);

	m_psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
}

void GraphicsPSO::CompilePixelShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_)
{
	ID3DBlob* pixelShader;

	if (FAILED(D3DCompileFromFile(file_, NULL, NULL, entryPoint_, targetProfile_, D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &pixelShader, NULL)))
		Debug::Error("Could not create pixel shader", __FILENAME__, __LINE__);

	m_psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
}

void GraphicsPSO::Finalize()
{
	m_psoDesc.pRootSignature = m_rootSignature->GetRootSignature();
	ASSERT(m_psoDesc.pRootSignature != nullptr, "If we're finalizing the pso, the root sig should not be null");

	m_psoDesc.InputLayout.pInputElementDescs = m_inputLayouts.get();

	if (FAILED(DXGraphics::m_device->CreateGraphicsPipelineState(&m_psoDesc, IID_PPV_ARGS(&m_pso))))
		Debug::FatalError("Graphics PSO could not be created!", __FILENAME__, __LINE__);
}

ComputePSO::ComputePSO()
{
	memset(&m_computeDesc, 0, sizeof(m_computeDesc));
	m_computeDesc.NodeMask = 1;
}

void ComputePSO::CompileComputeShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_)
{
	ID3DBlob* computeShader;
	if (FAILED(D3DCompileFromFile(file_, NULL, NULL, entryPoint_, targetProfile_, D3DCOMPILE_ALL_RESOURCES_BOUND, 0, &computeShader, NULL)))
		ERROR("Could not create compute shader");

	m_computeDesc.CS = { reinterpret_cast<UINT8*>(computeShader->GetBufferPointer()), computeShader->GetBufferSize() };
}

void ComputePSO::Finalize()
{
	m_computeDesc.pRootSignature = m_rootSignature->GetRootSignature();
	ASSERT(m_computeDesc.pRootSignature != nullptr, "Root Signature is null!");

	if (FAILED(DXGraphics::m_device->CreateComputePipelineState(&m_computeDesc, IID_PPV_ARGS(&m_pso))))
	{
		Debug::FatalError("Could not create compute shader pipeline!", __FILENAME__, __LINE__);
		throw std::runtime_error("Could not create compute shader pipeline!");
	}
}
