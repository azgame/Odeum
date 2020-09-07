#include "GraphicsContext.h"

// TODO Aidan: Make comments

CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type_)
{
    std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex);

    auto& AvailableContexts = sm_availableContexts[type_];

    CommandContext* ret = nullptr;
    if (AvailableContexts.empty())
    {
        ret = new CommandContext(type_);
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

void ContextManager::FreeContext(CommandContext* context_)
{
	assert(context_ != nullptr);
	std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex);
	sm_availableContexts[context_->m_type].push(context_);
}

void ContextManager::DestroyAllContexts()
{
    for (uint32_t i = 0; i < 4; ++i)
        sm_ContextPool[i].clear();
}

CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type_) :
    m_type(type_),
    m_dynamicViewDescHeap(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
    m_dynamicSamplerDescHeap(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
{
    m_manager = nullptr;
    m_commandList = nullptr;
    ZeroMemory(m_currentDescHeap, sizeof(m_currentDescHeap));

    m_curGraphicsRootSignature = nullptr;
    m_curComputeRootSignature = nullptr;
    m_curPipelineState = nullptr;
    m_numBarriersToFlush = 0;
}

CommandContext::~CommandContext()
{
}

CommandContext& CommandContext::Initialize(std::wstring name_)
{
    CommandContext* context = DXGraphics::m_contextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
    // set id
    return *context;
}

void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_)
{
    
}

void CommandContext::Reset()
{
}
