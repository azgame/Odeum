#include "DescriptorAllocator.h"

#include "D3DCore.h"

using namespace DXGraphics;

std::mutex DescriptorAllocator::m_allocatorMutex;
std::vector<ID3D12DescriptorHeap*> DescriptorAllocator::m_descriptorHeapPool;

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::Allocate(UINT count_)
{
	if (m_currentHeap == nullptr || m_numFreeHandles < count_)
	{
		m_currentHeap = GetNewHeap(m_type);
		m_currentHandle = m_currentHeap->GetCPUDescriptorHandleForHeapStart();
		m_numFreeHandles = kDescriptorsPerHeap;

		if (m_descrptorSize == 0)
			m_descrptorSize = m_device->GetDescriptorHandleIncrementSize(m_type);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_currentHandle;
	m_currentHandle.ptr += count_ * m_descrptorSize;
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

	m_descriptorHeapPool.emplace_back(heap);
	
	return heap;
}
