#ifndef _DEVICERESOURCES_H_
#define _DEVICERESOURCES_H_

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "pch.h"

static const UINT c_frameCount = 2;		// Use triple buffering.

class DeviceResources
{
public:
	DeviceResources();
	DeviceResources(const DeviceResources&);
	~DeviceResources();

	bool Initialize(int, int, HWND, bool, bool);
	void InitializeFence();
	void Uninitialize();

	bool Render();
	bool WaitForPrevFrame();

	ID3D12Device* GetD3Device() const					{ return m_device; }
	ID3D12GraphicsCommandList* GetCommandList() const	{ return m_commandList; }
	ID3D12CommandAllocator* GetCommandAllocator() const	{ return m_commandAllocator; }
	ID3D12Resource* GetBackBuffer(int i) const			{ return m_backBufferRenderTarget[i]; }
	ID3D12DescriptorHeap* GetRTVHeap() const			{ return m_renderTargetViewHeap; }
	ID3D12CommandQueue* GetCommandQ() const				{ return m_commandQueue; }
	IDXGISwapChain3* GetSwapChain() const				{ return m_swapChain; }
	D3D12_VIEWPORT* GetViewPort() const					{ return m_viewPort; }
	D3D12_RECT* GetSSRect() const						{ return m_ssRect; }


private:
	bool								m_vsync_enabled;
	ID3D12Device*						m_device;
	ID3D12CommandQueue*					m_commandQueue;
	char								m_videoCardDescription[128];
	IDXGISwapChain3*					m_swapChain;
	ID3D12DescriptorHeap*				m_renderTargetViewHeap;
	ID3D12Resource*						m_backBufferRenderTarget[c_frameCount];
	unsigned int						m_bufferIndex;
	ID3D12CommandAllocator*				m_commandAllocator;
	ID3D12GraphicsCommandList*			m_commandList;
	ID3D12PipelineState*				m_pipelineState;
	ID3D12Fence*						m_fence;
	HANDLE								m_fenceEvent;
	UINT64								m_fenceValues[c_frameCount];
	bool								m_deviceRemoved;
	UINT								m_currentFrame;
	D3D12_VIEWPORT*						m_viewPort;
	D3D12_RECT*							m_ssRect;
	UINT64								m_fenceValue;
};

#endif // !_DEVICERESOURCES_H_