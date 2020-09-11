#ifndef D3DCORE_H
#define D3DCORE_H

#include "DXIncludes.h"

#include "DescriptorAllocator.h"

class CommandListManager;
class ContextManager;

namespace DXGraphics
{
	void Initialize();
	void Resize(uint32_t width_, uint32_t height_);
	void Shutdown();
	void Present();

	extern uint32_t m_displayWidth;
	extern uint32_t m_displayHeight;

	uint64_t GetFrameCount();
	float GetFrameTime();
	float GetFrameRate();

	extern ID3D12Device5* m_device;
	extern CommandListManager m_commandManager;
	extern ContextManager m_contextManager;

	extern DescriptorAllocator m_descriptorAllocators[];

	inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type_, UINT count_ = 1)
	{
		return m_descriptorAllocators[type_].Allocate(count_);
	}

	enum ResolutionOptions { k720p, k900p, k1080p, k1440p, k1800p, k2160p };

	extern bool s_enableVSync;
	extern ResolutionOptions m_targetResolution;
	extern bool s_ultraWide;
}

#endif