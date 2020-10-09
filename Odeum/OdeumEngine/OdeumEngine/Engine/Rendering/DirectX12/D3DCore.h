#ifndef D3DCORE_H
#define D3DCORE_H

#include "D3DIncludes.h"

#include "DescriptorAllocator.h"


class CommandListManager;
class ContextManager;
class ColourBuffer;
class DepthBuffer;

#define SWAP_CHAIN_BUFFER_COUNT 3

// Global dx rendering state
namespace DXGraphics
{
	void Initialize();
	// void InitializeRenderingBuffers(uint32_t nativeWidth_, uint32_t nativeHeight_);
	void Resize(uint32_t width_, uint32_t height_);
	void Shutdown();
	void Present();

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
	extern ColourBuffer m_overlayBuffer;
	extern DepthBuffer m_sceneDepthBuffer;

	extern D3D12_BLEND_DESC alphaBlend;
	extern D3D12_RASTERIZER_DESC rasterDesc;
	extern D3D12_DEPTH_STENCIL_DESC depthReadWrite;
}

#endif