#include "BufferAllocator.h"

#include "GraphicsContext.h"

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

	}
}

BufferPage* BufferPageManager::CreateNewPage(size_t size_)
{
	return nullptr;
}

void BufferPageManager::RecyclePages(uint64_t fenceID_, const std::vector<BufferPage*>& pages_)
{
}

AllocatedBuffer BufferAllocator::Allocate(size_t byteSize_, size_t alignment_)
{
	ALIGN(byteSize_, alignment_);
}

void BufferAllocator::CleanupUsedPages(uint64_t fenceID_)
{
}
