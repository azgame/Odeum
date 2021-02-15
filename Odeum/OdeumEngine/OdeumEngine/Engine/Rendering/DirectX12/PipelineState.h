#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

// Comments by Aidan Zizys, 2020

#include "D3DIncludes.h"

class RootSignature;
class CommandContext;
class GraphicsContext;

// PSO and derived classes used to wrap pipeline states in an interface
class PSO
{
	friend class CommandContext;
public:

	PSO() : m_rootSignature(nullptr), m_pso(nullptr) {}

	void Destroy();

	void SetRootSignature(const RootSignature& rootSig)
	{
		m_rootSignature = &rootSig;
	}

	const RootSignature& GetRootSignature() const
	{
        ASSERT(m_rootSignature != nullptr, "If we're retrieving the root sig for use, it can't be null");
		return *m_rootSignature;
	}

	ID3D12PipelineState* GetPipelineState() const { return m_pso; }

protected:

	const RootSignature* m_rootSignature;

	ID3D12PipelineState* m_pso;
};

class GraphicsPSO : public PSO
{
	friend class GraphicsContext;
public:

	GraphicsPSO();

    void SetBlendState(const D3D12_BLEND_DESC& blendDesc_);
    void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc_);
    void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc_);
    void SetSampleMask(UINT sampleMask_);
    void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType_);
    void SetRenderTargetFormat(DXGI_FORMAT rtvFormat_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_ = 1, UINT MsaaQuality_ = 0);
    void SetRenderTargetFormats(UINT numRTVs_, const DXGI_FORMAT* rtvFormats_, DXGI_FORMAT dsvFormat_, UINT MsaaCount_ = 1, UINT MsaaQuality_ = 0);
    void SetInputLayout(UINT numElements_, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs_);

    void CompileVertexShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_);
    void CompilePixelShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_);

    void SetVertexShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.VS = binary_; }
    void SetPixelShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.PS = binary_; }
    void SetGeometryShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.GS = binary_; }
    void SetHullShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.HS = binary_; }
    void SetDomainShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.DS = binary_; }

    void Finalize();

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> m_inputLayouts;
};

class ComputePSO : public PSO
{
    friend class CommandContext;

public:
    ComputePSO();

    void CompileComputeShader(LPCWSTR file_, LPCSTR entryPoint_, LPCSTR targetProfile_);

    void SetComputeShader(const D3D12_SHADER_BYTECODE& binary_) { m_computeDesc.CS = binary_; }

    void Finalize();

private:
    D3D12_COMPUTE_PIPELINE_STATE_DESC m_computeDesc;
};

#endif