#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "PixelBuffer.h"

class DepthBuffer : public PixelBuffer
{
public:
	DepthBuffer(float clearDepth_ = 0.0f, uint8_t clearStencil_ = 0)
		: m_clearDepth(clearDepth_), m_clearStencil(clearStencil_)
	{
		m_dsvHandle[0].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_dsvHandle[1].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_dsvHandle[2].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_dsvHandle[3].ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_depthSRVHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_stencilSRVHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	void Create(const std::wstring& name_, uint32_t width_, uint32_t height_, DXGI_FORMAT format_,
		uint32_t numSamples_ = 1, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV() const { return m_dsvHandle[0]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_DepthReadOnly() const { return m_dsvHandle[1]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_StencilReadOnly() const { return m_dsvHandle[2]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV_ReadOnly() const { return m_dsvHandle[3]; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthSRV() const { return m_depthSRVHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetStencilSRV() const { return m_stencilSRVHandle; }

	float GetClearDepth() const { return m_clearDepth; }
	uint8_t GetClearStencil() const { return m_clearStencil; }

private:

	void CreateDerivedViews(ID3D12Device* device_, DXGI_FORMAT format_);

	float m_clearDepth;
	uint8_t m_clearStencil;

	D3D12_CPU_DESCRIPTOR_HANDLE m_dsvHandle[4];
	D3D12_CPU_DESCRIPTOR_HANDLE m_depthSRVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_stencilSRVHandle;

};

#endif