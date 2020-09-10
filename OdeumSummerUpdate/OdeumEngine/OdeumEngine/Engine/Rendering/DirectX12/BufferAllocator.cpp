#include "BufferAllocator.h"

#include "DXCommandContext.h"

BufferAllocatorType BufferPageManager::sm_initType = gpuExclusive;

BufferPageManager::BufferPageManager()
{
	m_allocationType = sm_initType;
	sm_initType = (BufferAllocatorType)(sm_initType + 1);
	ASSERT(sm_initType <= numAllocatorTypes);
}

BufferPageManager BufferAllocator::sm_pageManager[2];

BufferPage* BufferPageManager::RequestPage()
{
	std::lock_guard<std::mutex> LockGuard(m_mutex);

	while (!m_retiredPages.empty() && DXGraphics::m_commandManager.IsFenceComplete(m_retiredPages.front().first))
	{
		m_availablePages.push(m_retiredPages.front().second);
		m_retiredPages.pop();
	}

	BufferPage* page = nullptr;

	if (!m_availablePages.empty())
	{
		page = m_availablePages.front();
		m_availablePages.pop();
	}
	else
	{
		page = CreateNewPage();
		m_pagePool.emplace_back(page);
	}

	return page;
}

BufferPage* BufferPageManager::CreateNewPage(size_t size_)
{
    D3D12_HEAP_PROPERTIES heapProps;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_RESOURCE_STATES defaultUsage;

    if (m_allocationType == gpuExclusive)
    {
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        resourceDesc.Width = size_ == 0 ? gpuAllocatorPageSize : size_;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        defaultUsage = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    }
    else
    {
        heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
        resourceDesc.Width = size_ == 0 ? cpuAllocatorPageSize : size_;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        defaultUsage = D3D12_RESOURCE_STATE_GENERIC_READ;
    }

    ID3D12Resource* buffer;
    if (FAILED(DXGraphics::m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        defaultUsage,
        nullptr,
        IID_PPV_ARGS(&buffer)))
    )
        Debug::Error("Could not create buffer page", __FILENAME__, __LINE__);

    return new BufferPage(buffer, defaultUsage);
}

void BufferPageManager::RecyclePages(uint64_t fenceID_, const std::vector<BufferPage*>& pages_)
{
	std::lock_guard<std::mutex> LockGuard(m_mutex);
	for (auto page : pages_)
		m_retiredPages.push(std::make_pair(fenceID_, page));
}

AllocatedBuffer BufferAllocator::Allocate(size_t byteSize_, size_t alignment_)
{
	ALIGN(byteSize_, alignment_);
    ALIGN(m_offset, alignment_);

    if (m_offset + byteSize_ > m_pageSize)
    {
        ASSERT(m_page != nullptr, "Buffer page is null!");
        m_retiredPages.push_back(m_page);
        m_page = nullptr;
    }

    if (m_page == nullptr)
    {
        m_page = sm_pageManager[m_allocationType].RequestPage();
        m_offset = 0;
    }

    AllocatedBuffer returnBuffer(*m_page, m_offset, byteSize_);
    returnBuffer.CpuAddress = (uint8_t*)m_page->m_CpuVirtualAddress + m_offset;
    returnBuffer.GpuAddress = m_page->m_GpuVirtualAddress + m_offset;

    m_offset += byteSize_;

    return returnBuffer;
}

void BufferAllocator::CleanupUsedPages(uint64_t fenceID_)
{
    if (m_page == nullptr)
        return;

    m_retiredPages.push_back(m_page);
    m_page = nullptr;
    m_offset = 0;

    sm_pageManager[m_allocationType].RecyclePages(fenceID_, m_retiredPages);
    m_retiredPages.clear();
}
