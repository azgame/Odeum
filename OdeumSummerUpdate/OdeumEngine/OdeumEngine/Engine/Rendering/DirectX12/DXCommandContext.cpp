#include "DXCommandContext.h"

#include "Buffers/ColourBuffer.h"
#include "Buffers/DepthBuffer.h"

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
    m_dynamicSamplerDescHeap(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER),
    m_CpuBufferAllocator(cpuWritable),
    m_GpuBufferAllocator(gpuExclusive)
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

void CommandContext::DestroyAllContexts()
{
    BufferAllocator::DestroyAllPages();
    DynamicDescriptorHeap::Destroy();
    DXGraphics::m_contextManager.DestroyAllContexts();
}

// Ask for an available context
CommandContext& CommandContext::RequestContext(std::wstring name_)
{
    CommandContext* context = DXGraphics::m_contextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
    context->m_id = name_;
    return *context;
}

uint64_t CommandContext::Flush(bool waitForCompletion_)
{
    FlushResourceBarriers();

    ASSERT(m_currentAllocator != nullptr, "Allocator is null!");

    uint64_t fenceValue = DXGraphics::m_commandManager.GetQueue(m_type).ExecuteCommandList(m_commandList);

    if (waitForCompletion_)
        DXGraphics::m_commandManager.WaitForFence(fenceValue);

    m_commandList->Reset(m_currentAllocator, nullptr);

    if (m_curGraphicsRootSignature)
        m_commandList->SetGraphicsRootSignature(m_curGraphicsRootSignature);

    if (m_curComputeRootSignature)
        m_commandList->SetComputeRootSignature(m_curComputeRootSignature);

    if (m_curPipelineState)
        m_commandList->SetPipelineState(m_curPipelineState);

    BindDescriptorHeaps();

    return fenceValue;
}

uint64_t CommandContext::Finish(bool waitForCompletion_)
{
    ASSERT(m_type == D3D12_COMMAND_LIST_TYPE_DIRECT || m_type == D3D12_COMMAND_LIST_TYPE_COMPUTE);

    FlushResourceBarriers();

    ASSERT(m_currentAllocator != nullptr, "Command Allocator is null!");

    CommandQueue& queue = DXGraphics::m_commandManager.GetQueue(m_type);

    uint64_t fenceValue = queue.ExecuteCommandList(m_commandList);
    queue.DiscardAllocator(fenceValue, m_currentAllocator);
    m_currentAllocator = nullptr;

    m_CpuBufferAllocator.CleanupUsedPages(fenceValue);
    m_GpuBufferAllocator.CleanupUsedPages(fenceValue);
    m_dynamicViewDescHeap.CleanupUsedHeaps(fenceValue);
    m_dynamicSamplerDescHeap.CleanupUsedHeaps(fenceValue);

    if (waitForCompletion_)
        DXGraphics::m_commandManager.WaitForFence(fenceValue);

    DXGraphics::m_contextManager.FreeContext(this);

    return fenceValue;
}

void CommandContext::Initialize()
{
    DXGraphics::m_commandManager.CreateNewCommandList(m_type, &m_commandList, &m_currentAllocator);
}

void CommandContext::CopySubresource(D3DResource& dest_, UINT destSubIndex_, D3DResource& src_, UINT srcSubIndex_)
{
    D3D12_TEXTURE_COPY_LOCATION destLocation =
    {
        dest_.GetResource(),
        D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
        destSubIndex_
    };

    D3D12_TEXTURE_COPY_LOCATION srcLocation =
    {
        src_.GetResource(),
        D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
        srcSubIndex_
    };

    m_commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);
}

void CommandContext::InitializeTexture(D3DResource& dest_, UINT numSubresources_, D3D12_SUBRESOURCE_DATA subResource_[])
{
    UINT64 uploadBufferSize = GetRequiredIntermediateSize(dest_.GetResource(), 0, numSubresources_);

    CommandContext& context = CommandContext::RequestContext();

    AllocatedBuffer buffer = context.ReserveBufferMemory(uploadBufferSize);
    UpdateSubresources(context.m_commandList, dest_.GetResource(), buffer.buffer.GetResource(), 0, 0, numSubresources_, subResource_);
    context.TransitionResource(dest_, D3D12_RESOURCE_STATE_GENERIC_READ);

    context.Finish(true);
}

void CommandContext::InitializeBuffer(D3DResource& dest_, const void* pData_, size_t numBytes_, size_t offset_)
{
    CommandContext& context = CommandContext::RequestContext();

    AllocatedBuffer buffer = context.ReserveBufferMemory(numBytes_);
    memcpy(buffer.CpuAddress, pData_, (numBytes_ + 15) / 16);

    context.TransitionResource(dest_, D3D12_RESOURCE_STATE_COPY_DEST, true);
    context.m_commandList->CopyBufferRegion(dest_.GetResource(), offset_, buffer.buffer.GetResource(), 0, numBytes_);
    context.TransitionResource(dest_, D3D12_RESOURCE_STATE_GENERIC_READ, true);

    context.Finish(true);
}

void CommandContext::InitializeTextureArraySlice(D3DResource& dest_, UINT sliceIndex_, D3DResource& src_)
{
    CommandContext& Context = CommandContext::RequestContext();

    Context.TransitionResource(dest_, D3D12_RESOURCE_STATE_COPY_DEST);
    Context.FlushResourceBarriers();

    const D3D12_RESOURCE_DESC& DestDesc = dest_.GetResource()->GetDesc();
    const D3D12_RESOURCE_DESC& SrcDesc = src_.GetResource()->GetDesc();

    ASSERT(sliceIndex_ < DestDesc.DepthOrArraySize&&
        SrcDesc.DepthOrArraySize == 1 &&
        DestDesc.Width == SrcDesc.Width &&
        DestDesc.Height == SrcDesc.Height &&
        DestDesc.MipLevels <= SrcDesc.MipLevels, "Something about the descriptions is wrong"
    );

    UINT SubResourceIndex = sliceIndex_ * DestDesc.MipLevels;

    for (UINT i = 0; i < DestDesc.MipLevels; ++i)
    {
        D3D12_TEXTURE_COPY_LOCATION destCopyLocation =
        {
            dest_.GetResource(),
            D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
            SubResourceIndex + i
        };

        D3D12_TEXTURE_COPY_LOCATION srcCopyLocation =
        {
            src_.GetResource(),
            D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
            i
        };

        Context.m_commandList->CopyTextureRegion(&destCopyLocation, 0, 0, 0, &srcCopyLocation, nullptr);
    }

    Context.TransitionResource(dest_, D3D12_RESOURCE_STATE_GENERIC_READ);
    Context.Finish(true);
}

void CommandContext::WriteBuffer(D3DResource& dest_, size_t destOffset_, const void* pData_, size_t numBytes_)
{
    ASSERT(pData_ != nullptr && isAligned(pData_, 16));
    AllocatedBuffer tempSpace = m_CpuBufferAllocator.Allocate(numBytes_, 512);
    memcpy(tempSpace.CpuAddress, pData_, (numBytes_ + 15) / 16);
    CopyBufferRegion(dest_, destOffset_, tempSpace.buffer, tempSpace.offset, numBytes_);
}

void CommandContext::TransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow)
{
    D3D12_RESOURCE_STATES prevState = resource_.m_usageState;

    if (m_type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
    {
        ASSERT((prevState & VALID_COMPUTE_QUEUE_RESOURCE_STATES) == prevState);
        ASSERT((newState_ & VALID_COMPUTE_QUEUE_RESOURCE_STATES) == newState_);
    }

    if (prevState != newState_)
    {
        ASSERT(m_numBarriersToFlush < 16, "Exceeded arbitrary limit on buffered barriers");
        D3D12_RESOURCE_BARRIER& BarrierDesc = m_barrierBuffer[m_numBarriersToFlush++];

        BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Transition.pResource = resource_.GetResource();
        BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        BarrierDesc.Transition.StateBefore = prevState;
        BarrierDesc.Transition.StateAfter = newState_;

        // Check to see if we already started the transition
        if (newState_ == resource_.m_transitioningState)
        {
            BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
            resource_.m_transitioningState = (D3D12_RESOURCE_STATES)-1;
        }
        else
            BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

        resource_.m_usageState = newState_;
    }
    else if (newState_ == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
        InsertUAVBarrier(resource_, flushNow);

    if (flushNow || m_numBarriersToFlush == 16)
        FlushResourceBarriers();
}

void CommandContext::BeginTransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow)
{
    if (resource_.m_transitioningState != (D3D12_RESOURCE_STATES)-1)
        TransitionResource(resource_, resource_.m_transitioningState);

    D3D12_RESOURCE_STATES prevState = resource_.m_usageState;

    if (prevState != newState_)
    {
        ASSERT(m_numBarriersToFlush < 16, "Exceeded arbitrary limit on buffered barriers");
        D3D12_RESOURCE_BARRIER& BarrierDesc = m_barrierBuffer[m_numBarriersToFlush++];

        BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        BarrierDesc.Transition.pResource = resource_.GetResource();
        BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        BarrierDesc.Transition.StateBefore = prevState;
        BarrierDesc.Transition.StateAfter = newState_;

        BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

        resource_.m_transitioningState = newState_;
    }

    if (flushNow || m_numBarriersToFlush == 16)
        FlushResourceBarriers();
}

void CommandContext::InsertUAVBarrier(D3DResource& resource_, bool flushNow)
{
    ASSERT(m_numBarriersToFlush < 16, "Can't have more than 16 barriers");
    D3D12_RESOURCE_BARRIER& barrierDesc = m_barrierBuffer[m_numBarriersToFlush];

    barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrierDesc.UAV.pResource = resource_.GetResource();

    if (flushNow)
        FlushResourceBarriers();
}

void CommandContext::InsertAliasBuffer(D3DResource& before_, D3DResource after_, bool flushNow)
{
    ASSERT(m_numBarriersToFlush < 16, "Can't have more than 16 barriers");
    D3D12_RESOURCE_BARRIER& barrierDesc = m_barrierBuffer[m_numBarriersToFlush++];

    barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
    barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrierDesc.Aliasing.pResourceBefore = before_.GetResource();
    barrierDesc.Aliasing.pResourceAfter = after_.GetResource();

    if (flushNow)
        FlushResourceBarriers();
}

void CommandContext::BindDescriptorHeaps()
{
    UINT nonNullHeaps = 0;
    ID3D12DescriptorHeap* heapsForBinding[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
    for (UINT i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; i++)
    {
        ID3D12DescriptorHeap* heapIterator = m_currentDescHeaps[i];
        if (heapIterator != nullptr)
            heapsForBinding[nonNullHeaps++] = heapIterator;
    }

    if (nonNullHeaps > 0)
        m_commandList->SetDescriptorHeaps(nonNullHeaps, heapsForBinding);
}

void CommandContext::Reset()
{
    ASSERT(m_commandList != nullptr && m_currentAllocator == nullptr, "Context has not been freed");
    m_currentAllocator = DXGraphics::m_commandManager.GetQueue(m_type).RequestAllocator();
    m_commandList->Reset(m_currentAllocator, nullptr);

    m_curGraphicsRootSignature = nullptr;
    m_curPipelineState = nullptr;
    m_curComputeRootSignature = nullptr;
    m_numBarriersToFlush = 0;

    BindDescriptorHeaps();
}

void GraphicsContext::ClearUAV(D3DBuffer& target_)
{
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_dynamicViewDescHeap.UploadDirect(target_.GetUAV());
    const UINT clearColor[4] = {};
    m_commandList->ClearUnorderedAccessViewUint(gpuHandle, target_.GetUAV(), target_.GetResource(), clearColor, 0, nullptr);
}

void GraphicsContext::ClearUAV(ColourBuffer& target_)
{
    D3D12_GPU_DESCRIPTOR_HANDLE GpuVisibleHandle = m_dynamicViewDescHeap.UploadDirect(target_.GetUAV());
    CD3DX12_RECT clearRect(0, 0, target_.GetWidth(), target_.GetHeight());

    const float* clearColor = target_.GetClearColour().GetPtr();
    m_commandList->ClearUnorderedAccessViewFloat(GpuVisibleHandle, target_.GetUAV(), target_.GetResource(), clearColor, 1, &clearRect);
}

void GraphicsContext::ClearColor(ColourBuffer& target_)
{
    m_commandList->ClearRenderTargetView(target_.GetRTV(), target_.GetClearColour().GetPtr(), 0, nullptr);
}

void GraphicsContext::ClearDepth(DepthBuffer& target_)
{
    m_commandList->ClearDepthStencilView(target_.GetDSV(), D3D12_CLEAR_FLAG_DEPTH, target_.GetClearDepth(), target_.GetClearStencil(), 0, nullptr);
}

void GraphicsContext::ClearStencil(DepthBuffer& target_)
{
    m_commandList->ClearDepthStencilView(target_.GetDSV(), D3D12_CLEAR_FLAG_STENCIL, target_.GetClearDepth(), target_.GetClearStencil(), 0, nullptr);
}

void GraphicsContext::ClearDepthAndStencil(DepthBuffer& target_)
{
    m_commandList->ClearDepthStencilView(target_.GetDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, target_.GetClearDepth(), target_.GetClearStencil(), 0, nullptr);
}

void GraphicsContext::SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[])
{
    m_commandList->OMSetRenderTargets(numRTVs_, rtvs_, false, nullptr);
}

void GraphicsContext::SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[], D3D12_CPU_DESCRIPTOR_HANDLE dsv_)
{
    m_commandList->OMSetRenderTargets(numRTVs_, rtvs_, false, &dsv_);
}

void GraphicsContext::SetViewport(const D3D12_VIEWPORT& viewport_)
{
    m_commandList->RSSetViewports(1, &viewport_);
}

void GraphicsContext::SetViewport(FLOAT x_, FLOAT y_, FLOAT w_, FLOAT h_, FLOAT minDepth_, FLOAT maxDepth_)
{
    D3D12_VIEWPORT viewport;
    viewport.Width = w_;
    viewport.Height = h_;
    viewport.MinDepth = minDepth_;
    viewport.MaxDepth = maxDepth_;
    viewport.TopLeftX = x_;
    viewport.TopLeftY = y_;
    m_commandList->RSSetViewports(1, &viewport);
}

void GraphicsContext::SetScissor(const D3D12_RECT& rect_)
{
    ASSERT(rect_.left < rect_.right&& rect_.top < rect_.bottom, "Scissor rect is twisted");
    m_commandList->RSSetScissorRects(1, &rect_);
}

void GraphicsContext::SetViewportAndScissor(const D3D12_VIEWPORT& viewport_, const D3D12_RECT& rect_)
{
    ASSERT(rect_.left < rect_.right&& rect_.top < rect_.bottom, "Scissor rect is twisted");
    m_commandList->RSSetViewports(1, &viewport_);
    m_commandList->RSSetScissorRects(1, &rect_);
}