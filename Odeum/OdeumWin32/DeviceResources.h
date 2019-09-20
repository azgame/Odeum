#ifndef _DEVICERESOURCES_H_
#define _DEVICERESOURCES_H_

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "pch.h"

class DeviceResources
{
public:
	DeviceResources();
	DeviceResources(const DeviceResources&);
	~DeviceResources();

	bool Initialize(int, int, HWND, bool, bool);
	void Uninitialize();

	bool Render();

private:
	bool m_vsync_enabled;
	ID3D12Device* m_device;
	ID3D12CommandQueue* m_commandQueue;
	char m_videoCardDescription[128];
	unsigned int m_videoCardMemory;
	IDXGISwapChain3* m_swapChain;
	ID3D12DescriptorHeap* m_renderTargetViewHeap;
	ID3D12Resource* m_backBufferRenderTarget[2];
	unsigned int m_bufferIndex;
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;
	ID3D12PipelineState* m_pipelineState;
	ID3D12Fence* m_fence;
	HANDLE m_fenceEvent;
	unsigned long long m_fenceValue;
};

#endif // !_DEVICERESOURCES_H_