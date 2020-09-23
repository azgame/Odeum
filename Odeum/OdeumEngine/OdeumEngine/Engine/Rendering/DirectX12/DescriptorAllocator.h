#ifndef DESCRIPTORALLOCATOR_H
#define DESCRIPTORALLOCATOR_H

#include "DXIncludes.h"

// Acts as a place for allocating cpu visible descriptors, not for gpu use
// Has a pool of descriptors and can, in a thread safe way, allocate descriptors into that heap for use by other objects
class DescriptorAllocator
{
public:

	DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type_) : m_type(type_), m_currentHeap(nullptr) {}

	D3D12_CPU_DESCRIPTOR_HANDLE Allocate(UINT count_);

	static void DestroyHeaps();

private:

	// can be threaded
	static ID3D12DescriptorHeap* GetNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_);
	static const uint32_t kDescriptorsPerHeap = 256;
	static std::vector<ID3D12DescriptorHeap*> m_descriptorHeapPool;
	static std::mutex m_allocatorMutex;

	ID3D12DescriptorHeap* m_currentHeap;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentHandle;
	uint32_t m_descriptorSize;
	uint32_t m_numFreeHandles;
};

class DescriptorHandle
{
public:
    DescriptorHandle()
    {
        m_cpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_gpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }

    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle)
        : m_cpuHandle(CpuHandle)
    {
        m_gpuHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
    }

    DescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE GpuHandle)
        : m_cpuHandle(CpuHandle), m_gpuHandle(GpuHandle)
    {
    }

    DescriptorHandle operator+ (int OffsetScaledByDescriptorSize) const
    {
        DescriptorHandle ret = *this;
        ret += OffsetScaledByDescriptorSize;
        return ret;
    }

    void operator += (int OffsetScaledByDescriptorSize)
    {
        if (m_cpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
            m_cpuHandle.ptr += OffsetScaledByDescriptorSize;
        if (m_gpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
            m_gpuHandle.ptr += OffsetScaledByDescriptorSize;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const { return m_cpuHandle; }

    D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return m_gpuHandle; }

    bool isNull() const { return m_cpuHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }
    bool isShaderVisible() const { return m_gpuHandle.ptr != D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; }

private:
    D3D12_CPU_DESCRIPTOR_HANDLE m_cpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpuHandle;
};

#endif