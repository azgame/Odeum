#ifndef D3DCORE_H
#define D3DCORE_H

#include "DXIncludes.h"

#include "DescriptorAllocator.h"

class CommandListManager;
class ContextManager;
class ColourBuffer;
class DepthBuffer;

#define SWAP_CHAIN_BUFFER_COUNT 3

namespace DXGraphics
{
	void Initialize();
	void InitializeRenderingBuffers(uint32_t nativeWidth_, uint32_t nativeHeight_);
	void Resize(uint32_t width_, uint32_t height_);
	void Shutdown();
	void Present();

	extern uint32_t m_displayWidth;
	extern uint32_t m_displayHeight;

	extern UINT m_currentBuffer;

	uint64_t GetFrameCount();
	float GetFrameTime();
	float GetFrameRate();

	extern ID3D12Device* m_device;
	extern CommandListManager m_commandManager;
	extern ContextManager m_contextManager;

	extern DescriptorAllocator m_descriptorAllocators[];

	inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type_, UINT count_ = 1)
	{
		return m_descriptorAllocators[type_].Allocate(count_);
	}

	extern ColourBuffer m_displayPlane[SWAP_CHAIN_BUFFER_COUNT];
	extern ColourBuffer m_presentBuffer;
	extern DepthBuffer m_sceneDepthBuffer;

	enum ResolutionOptions { k720p, k900p, k1080p, k1440p, k1800p, k2160p };

	extern bool s_enableVSync;
	extern ResolutionOptions m_targetResolution;
	extern bool s_ultraWide;

	extern D3D12_BLEND_DESC alphaBlend;
	extern D3D12_RASTERIZER_DESC rasterDesc;
	extern D3D12_DEPTH_STENCIL_DESC depthStateDisabled;
}

#endif