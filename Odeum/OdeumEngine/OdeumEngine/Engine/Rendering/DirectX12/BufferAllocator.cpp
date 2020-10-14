#include "BufferAllocator.h"

#include "CommandContext.h"

BufferAllocatorType BufferLedger::sm_initType = gpuExclusive;

BufferLedger::BufferLedger()
{
	m_allocationType = sm_initType;
	sm_initType = (BufferAllocatorType)(sm_initType + 1);
	ASSERT(sm_initType <= numAllocatorTypes);
}

BufferLedger BufferAllocator::sm_pageManager[2];

BufferPage* BufferLedger::RequestPage()
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

BufferPage* BufferLedger::CreateNewPage(size_t size_)
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

void BufferLedger::RecyclePages(uint64_t fenceID_, const std::vector<BufferPage*>& pages_)
{
	std::lock_guard<std::mutex> LockGuard(m_mutex);
	for (auto page : pages_)
		m_retiredPages.push(std::make_pair(fenceID_, page));
}

void BufferLedger::RecycleLargePages(uint64_t fenceID_, const std::vector<BufferPage*>& pages_)
{
    std::lock_guard<std::mutex> LockGuard(m_mutex);

    while (!m_deletionQueue.empty() && DXGraphics::m_commandManager.IsFenceComplete(m_deletionQueue.front().first))
    {
        delete m_deletionQueue.front().second;
        m_deletionQueue.pop();
    }

    for (auto iter = pages_.begin(); iter != pages_.end(); ++iter)
    {
        (*iter)->UnMap();
        m_deletionQueue.push(std::make_pair(fenceID_, *iter));
    }
}

BufferEntry BufferAllocator::Allocate(size_t byteSize_, size_t alignment_)
{
	byteSize_ = Utility::Align(byteSize_, alignment_);
    m_offset = Utility::Align(m_offset, alignment_);

    if (byteSize_ > m_pageSize)
        return AllocateLargePage(byteSize_);

    if (m_offset + byteSize_ > m_pageSize)
    {
        ASSERT(m_bookmark != nullptr, "Buffer page is null!");
        m_retiredPages.push_back(m_bookmark);
        m_bookmark = nullptr;
    }

    if (m_bookmark == nullptr)
    {
        m_bookmark = sm_pageManager[m_allocationType].RequestPage();
        m_offset = 0;
    }

    BufferEntry returnBuffer(*m_bookmark, m_offset, byteSize_);
    returnBuffer.CpuAddress = (uint8_t*)m_bookmark->m_CpuVirtualAddress + m_offset;
    returnBuffer.GpuAddress = m_bookmark->m_GpuVirtualAddress + m_offset;

    m_offset += byteSize_;

    return returnBuffer;
}

void BufferAllocator::CleanupUsedPages(uint64_t fenceID_)
{
    if (m_bookmark == nullptr)
        return;

    m_retiredPages.push_back(m_bookmark);
    m_bookmark = nullptr;
    m_offset = 0;

    sm_pageManager[m_allocationType].RecyclePages(fenceID_, m_retiredPages);
    m_retiredPages.clear();

    sm_pageManager[m_allocationType].RecycleLargePages(fenceID_, m_largePages);
    m_largePages.clear();
}

BufferEntry BufferAllocator::AllocateLargePage(size_t sizeInBytes)
{
    BufferPage* singleSheet = sm_pageManager[m_allocationType].CreateNewPage(sizeInBytes);
    m_largePages.push_back(singleSheet);

    BufferEntry ret(*singleSheet, 0, sizeInBytes);
    ret.CpuAddress = singleSheet->m_CpuVirtualAddress;
    ret.GpuAddress = singleSheet->m_GpuVirtualAddress;

    return ret;
}
