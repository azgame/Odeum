#include "DescriptorAllocator.h"

#include "Buffers/D3DResource.h"

#include "D3DCore.h"

using namespace DXGraphics;

std::mutex DescriptorAllocator::m_allocatorMutex; // thread lock
std::vector<ID3D12DescriptorHeap*> DescriptorAllocator::m_descriptorHeapPool;

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(UINT count_)
{
	if (m_currentHeap == nullptr || m_numFreeHandles < count_) // if no current heap or not enough free handles
	{
		m_currentHeap = GetNewHeap(m_type); // get a new heap
		m_currentHandle = m_currentHeap->GetCPUDescriptorHandleForHeapStart();
		m_numFreeHandles = kDescriptorsPerHeap;

		if (m_descriptorSize == 0)
			m_descriptorSize = m_device->GetDescriptorHandleIncrementSize(m_type);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_currentHandle; // get the current handle for return 
	m_currentHandle.ptr += count_ * m_descriptorSize; // increment the current handle by the count * descriptor size
	m_numFreeHandles -= count_;

	return handle;  
}

void DescriptorAllocator::DestroyHeaps()
{
	for (auto heap : m_descriptorHeapPool)
		heap->Release();

	m_descriptorHeapPool.clear();
}

ID3D12DescriptorHeap* DescriptorAllocator::GetNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_)
{
	std::lock_guard<std::mutex> LockGuard(m_allocatorMutex); // lock heaps for creation

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = type_;
	desc.NodeMask = 1;
	desc.NumDescriptors = kDescriptorsPerHeap;

	ID3D12DescriptorHeap* heap;

	if (FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap))))
		Debug::Error("Could not create descriptor heap!", __FILENAME__, __LINE__);

	m_descriptorHeapPool.emplace_back(heap); // Create new heap and add it to the pool
	
	return heap;
}