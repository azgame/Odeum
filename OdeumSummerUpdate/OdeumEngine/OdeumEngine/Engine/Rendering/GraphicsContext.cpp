#include "GraphicsContext.h"

// TODO Aidan: Make comments

// Get a new context
CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type_)
{
    std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex); // thread lock

    auto& AvailableContexts = sm_availableContexts[type_]; // get a queue of contexts for the given type

    CommandContext* ret = nullptr;
    if (AvailableContexts.empty()) // if there are no contexts
    {
        ret = new CommandContext(type_); // create a new one
        sm_ContextPool[type_].emplace_back(ret); // and add to pool
        ret->Initialize(); // Initialize the context
    }
    else
    {
        ret = AvailableContexts.front(); // get the first available context
        AvailableContexts.pop(); // remove from the list of available contexts
        ret->Reset(); // reset the state of the context
    }

    ASSERT(ret != nullptr, "The returned command context must not be null");
    ASSERT(ret->m_type == type_, "The returned command context must match the type of the given type");

    return ret;
}

void ContextManager::FreeContext(CommandContext* context_)
{
    ASSERT(context_ != nullptr, "The given context must not be null");
	std::lock_guard<std::mutex> LockGuard(sm_contextAllocationMutex); // thread lock
	sm_availableContexts[context_->m_type].push(context_); // Add the used context to the queue of available contexts
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
    ZeroMemory(m_currentDescHeaps, sizeof(m_currentDescHeaps));

    m_curGraphicsRootSignature = nullptr;
    m_curComputeRootSignature = nullptr;
    m_curPipelineState = nullptr;
    m_numBarriersToFlush = 0;
}

CommandContext::~CommandContext()
{
}

// Ask for an available context
CommandContext& CommandContext::RequestContext(std::wstring name_)
{
    CommandContext* context = DXGraphics::m_contextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
    context->m_id = name_;
    return *context;
}

void CommandContext::Initialize()
{
    DXGraphics::m_commandManager.CreateNewCommandList(m_type, &m_commandList, &m_currentAllocator);
}

void CommandContext::CopySubresource(D3DResource& dest_, UINT destSubIndex_, D3DResource& src_, UINT srcSubIndex)
{
}

void CommandContext::InitializeTexture(D3DResource& dest_, UINT numSubresources_, D3D12_SUBRESOURCE_DATA subResource_[])
{
}

void CommandContext::InitializeBuffer(D3DResource& dest_, const void* pData_, size_t numBytes_, size_t offset_)
{
}

void CommandContext::InitializeTextureArraySlice(D3DResource& dest_, UINT sliceIndex_, D3DResource& src_)
{
}

void CommandContext::WriteBuffer(D3DResource& dest_, size_t destOffset_, const void* pData_, size_t numBytes_)
{
}

void CommandContext::TransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow)
{
}

void CommandContext::BeginTransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow)
{
}

void CommandContext::InsertUAVBarrier(D3DResource& resource_, bool flushNow)
{
}

void CommandContext::InsertAliasBuffer(D3DResource& before_, D3DResource after_, bool flushNow)
{
}

void CommandContext::BindDescriptorHeaps()
{
}

void CommandContext::Reset()
{
}

void GraphicsContext::ClearUAV(D3DResource& target_)
{
}

void GraphicsContext::ClearUAV(ColourBuffer& target_)
{
}

void GraphicsContext::ClearColor(ColourBuffer& target_)
{
}

void GraphicsContext::ClearDepth(DepthBuffer& target_)
{
}

void GraphicsContext::ClearStencil(DepthBuffer& target_)
{
}

void GraphicsContext::ClearDepthAndStencil(DepthBuffer& target_)
{
}

void GraphicsContext::SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[])
{
}

void GraphicsContext::SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[], D3D12_CPU_DESCRIPTOR_HANDLE dsv_)
{
}

void GraphicsContext::SetViewport(const D3D12_VIEWPORT& viewport_)
{
}

void GraphicsContext::SetViewport(FLOAT x_, FLOAT y_, FLOAT w_, FLOAT h_, FLOAT minDepth_, FLOAT maxDepth_)
{
}

void GraphicsContext::SetScissor(const D3D12_RECT& rect_)
{
}

void GraphicsContext::SetViewportAndScissor(const D3D12_VIEWPORT& viewport_, const D3D12_RECT& rect_)
{
}

void GraphicsContext::SetDynamicSRV(UINT rootIndex_, size_t bufferSize_, const void* srvBufferData_)
{
}