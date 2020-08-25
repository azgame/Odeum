#include "GraphicsContext.h"

GraphicsContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type_)
{
    std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex);

    auto& AvailableContexts = sm_availableContexts[type_];

    GraphicsContext* ret = nullptr;
    if (AvailableContexts.empty())
    {
        ret = new GraphicsContext(type_);
        sm_ContextPool[type_].emplace_back(ret);
        ret->Initialize();
    }
    else
    {
        ret = AvailableContexts.front();
        AvailableContexts.pop();
        ret->Reset();
    }

    assert(ret != nullptr);
    assert(ret->m_type == type_);

    return ret;
}

void ContextManager::FreeContext(GraphicsContext* context_)
{
	assert(context_ != nullptr);
	std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex);
	sm_availableContexts[context_->m_type].push(context_);
}

void ContextManager::DestroyAllContexts()
{
	for (auto context : sm_ContextPool)
		context.clear();
}

GraphicsContext* GraphicsContext::Initialize(std::string name_)
{
	return nullptr;
}

void GraphicsContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_)
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::Reset()
{
}
