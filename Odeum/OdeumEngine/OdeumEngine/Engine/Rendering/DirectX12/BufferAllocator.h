#ifndef BUFFERALLOCATOR_H
#define BUFFERALLOCATOR_H

#include "D3DIncludes.h"

#include "Buffers/D3DBuffer.h"
#include <queue>

// Individually allocated buffer for cpu use
class BufferEntry
{
public:
	BufferEntry(D3DResource& resource_, size_t offset_, size_t size_) :
		buffer(resource_), offset(offset_), size(size_) {}

	D3DResource& buffer;
	size_t offset;
	size_t size;
	void* CpuAddress;
	D3D12_GPU_VIRTUAL_ADDRESS GpuAddress;
};

// Page of available buffer space
class BufferPage : public D3DResource
{
public:

	BufferPage(ID3D12Resource* resource_, D3D12_RESOURCE_STATES usage_) : D3DResource()
	{
		m_resource.Attach(resource_);
		m_usageState = usage_;
		m_GpuVirtualAddress = m_resource->GetGPUVirtualAddress();
		m_resource->Map(0, nullptr, &m_CpuVirtualAddress);
	}

	~BufferPage()
	{
		UnMap(); // RAII
	}

	void Map()
	{
		if (m_CpuVirtualAddress == nullptr)
		{
			m_resource->Map(0, nullptr, &m_CpuVirtualAddress);
		}
	}

	void UnMap()
	{
		if (m_CpuVirtualAddress != nullptr)
		{
			m_resource->Unmap(0, nullptr);
			m_CpuVirtualAddress = nullptr;
		}
	}

	void* m_CpuVirtualAddress;
	D3D12_GPU_VIRTUAL_ADDRESS m_GpuVirtualAddress;
};

enum BufferAllocatorType
{
	gpuExclusive = 0,
	cpuWritable = 1,
	numAllocatorTypes
};

enum
{
	gpuAllocatorPageSize = 0x10000,		// 64k
	cpuAllocatorPageSize = 0x200000		// 2mb
};

// Manager that manages a pool of available buffer pages and retired buffer pages, discarding retired ones when needed
class BufferLedger
{
public:
	BufferLedger();
	BufferPage* RequestPage();
	BufferPage* CreateNewPage(size_t size_ = 0);

	void RecyclePages(uint64_t fenceID_, const std::vector<BufferPage*>& pages_);

	void Destroy() { m_pagePool.clear(); }

private:

	static BufferAllocatorType sm_initType;

	BufferAllocatorType m_allocationType;
	std::vector<std::unique_ptr<BufferPage>> m_pagePool;
	std::queue<std::pair<uint64_t, BufferPage*>> m_retiredPages;
	std::queue<BufferPage*> m_availablePages;
	std::mutex m_mutex;
};

// Class that has one cpu visible page manager and one gpu visible page manager used to access thread safe pools of pages
// Class has a current available page to make allocations, and when done places it into the retired pages pile.
// On allocate can ask for a new page from the owned page manager in a thread safe way if conditions met (size of incoming buffer > remaining available page space)
class BufferAllocator
{
public:

	BufferAllocator(BufferAllocatorType type_) : m_allocationType(type_), m_pageSize(0), m_offset(~(size_t)0), m_bookmark(nullptr)
	{
		ASSERT(type_ > -1 && type_ < numAllocatorTypes, "Invalid buffer allocator type");
		m_pageSize = (type_ == gpuExclusive ? gpuAllocatorPageSize : cpuAllocatorPageSize);
	}

	BufferEntry Allocate(size_t byteSize_, size_t alignment = 256);

	void CleanupUsedPages(uint64_t fenceID_);
	static void DestroyAllPages()
	{
		sm_pageManager[0].Destroy();
		sm_pageManager[1].Destroy();
	}

private:

	static BufferLedger sm_pageManager[2]; // one cpu visible, one gpu visible, accessible across all buffer allocators

	BufferAllocatorType m_allocationType;
	size_t m_pageSize;
	size_t m_offset;
	BufferPage* m_bookmark;
	std::vector<BufferPage*> m_retiredPages;
};

#endif