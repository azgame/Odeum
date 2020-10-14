#ifndef DEPTHBUFFER_H
#define DEPTHBUFFER_H

#include "PixelBuffer.h"

class DepthBuffer : public PixelBuffer
{
public:
	DepthBuffer(float clearDepth_ = 0.0f, uint8_t clearStencil_ = 0)
		: m_clearDepth(clearDepth_), m_clearStencil(clearStencil_)
	{
		m_depthStencilHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_depthSRVHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_stencilSRVHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	void Create(const std::wstring& name_, uint32_t width_, uint32_t height_, DXGI_FORMAT format_,
		D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

	void Create(const std::wstring& name_, uint32_t width_, uint32_t height_, uint32_t numSamples_, 
		DXGI_FORMAT format_, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSV() const { return m_depthStencilHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthSRV() const { return m_depthSRVHandle; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetStencilSRV() const { return m_stencilSRVHandle; }

	float GetClearDepth() const { return m_clearDepth; }
	uint8_t GetClearStencil() const { return m_clearStencil; }

private:

	void CreateDerivedViews(ID3D12Device* device_, DXGI_FORMAT format_);

	float m_clearDepth;
	uint8_t m_clearStencil;

	D3D12_CPU_DESCRIPTOR_HANDLE m_depthStencilHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_depthSRVHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE m_stencilSRVHandle;

};

#endif