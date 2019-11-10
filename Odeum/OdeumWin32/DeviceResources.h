#ifndef _DEVICERESOURCES_H_
#define _DEVICERESOURCES_H_

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "pch.h"

static const UINT c_frameCount = 3;		// Use triple buffering.

class DeviceResources
{
public:
	DeviceResources();
	DeviceResources(const DeviceResources&);
	~DeviceResources();

	bool Initialize(int, int, HWND, bool, bool, bool);
	void InitializeFence();
	void Uninitialize();

	bool Render();
	bool WaitForPrevFrame();

	ID3D12Device5* GetD3Device()						{ return m_device; }
	ID3D12CommandAllocator* GetCommandAllocator()		{ return m_commandAllocator; }
	ID3D12Resource* GetBackBuffer(int i)				{ return m_backBufferRenderTarget[i]; }
	ID3D12DescriptorHeap* GetRTVHeap()					{ return m_renderTargetViewHeap; }
	ID3D12Resource*	GetDepthStencil()					{ return m_depthStencil; }
	ID3D12CommandQueue* GetCommandQ() 					{ return m_commandQueue; }
	IDXGISwapChain3* GetSwapChain() 					{ return m_swapChain; }
	D3D12_VIEWPORT GetViewPort()						{ return m_viewPort; }
	D3D12_RECT* GetSSRect()								{ return m_ssRect; }
	DXGI_FORMAT GetBackBufferFormat()					{ return m_backBufferFormat; }

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}


private:
	bool								m_vsync_enabled;
	ID3D12Device5*						m_device;
	ID3D12CommandQueue*					m_commandQueue;
	char								m_videoCardDescription[128];
	IDXGISwapChain3*					m_swapChain;
	ID3D12DescriptorHeap*				m_renderTargetViewHeap;
	ID3D12DescriptorHeap*				m_dsvHeap;
	ID3D12Resource*						m_backBufferRenderTarget[c_frameCount];
	ID3D12Resource*						m_depthStencil;
	unsigned int						m_bufferIndex;
	DXGI_FORMAT							m_backBufferFormat;
	DXGI_FORMAT							m_depthBufferFormat;
	ID3D12CommandAllocator*				m_commandAllocator;
	ID3D12PipelineState*				m_pipelineState;
	ID3D12Fence*						m_fence;
	HANDLE								m_fenceEvent;
	bool								m_deviceRemoved;
	UINT								m_currentFrame;
	D3D12_VIEWPORT						m_viewPort;
	D3D12_RECT*							m_ssRect;
	UINT64								m_fenceValue;
	UINT64								m_fenceValues[c_frameCount];
};

#endif // !_DEVICERESOURCES_H_