#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "../../pch.h"

class RootSignature;
class GraphicsContext;

class PSO
{
	friend class GraphicsContext;
public:

	PSO() : m_rootSignature(nullptr) {}

	void Destroy();

	void SetRootSignature(const RootSignature& rootSig)
	{
		m_rootSignature = &rootSig;
	}

	const RootSignature& GetRootSignature() const
	{
		assert(m_rootSignature != nullptr);
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
    void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE indexBufferProps_);

    // These const_casts shouldn't be necessary, but we need to fix the API to accept "const void* pShaderBytecode"
    void SetVertexShader(const void* binary_, size_t size_) { m_psoDesc.VS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary_), size_); }
    void SetPixelShader(const void* binary_, size_t size_) { m_psoDesc.PS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary_), size_); }
    void SetGeometryShader(const void* binary_, size_t size_) { m_psoDesc.GS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary_), size_); }
    void SetHullShader(const void* binary_, size_t size_) { m_psoDesc.HS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary_), size_); }
    void SetDomainShader(const void* binary_, size_t size_) { m_psoDesc.DS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary_), size_); }

    void SetVertexShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.VS = binary_; }
    void SetPixelShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.PS = binary_; }
    void SetGeometryShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.GS = binary_; }
    void SetHullShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.HS = binary_; }
    void SetDomainShader(const D3D12_SHADER_BYTECODE& binary_) { m_psoDesc.DS = binary_; }

    // Perform validation and compute a hash value for fast state block comparisons
    void Finalize();

private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;
	std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> m_inputLayouts;
};

// class ComputePSO

#endif