#ifndef DESCRIPTORALLOCATOR_H
#define DESCRIPTORALLOCATOR_H

#include "../../pch.h"

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
	uint32_t m_descrptorSize;
	uint32_t m_numFreeHandles;
};

#endif