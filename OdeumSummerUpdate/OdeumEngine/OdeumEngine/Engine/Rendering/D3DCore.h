#ifndef D3DCORE_H
#define D3DCORE_H

#include "../../pch.h"

#include "DescriptorAllocator.h"
#include "CommandListManager.h"

namespace DXGraphics
{
	extern ID3D12Device5* m_device;
	extern CommandListManager m_commandManager;
	extern DescriptorAllocator m_descriptorAllocators[];

	inline D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type_, UINT count_ = 1)
	{
		return m_descriptorAllocators[type_].Allocate(count_);
	}
}

#endif