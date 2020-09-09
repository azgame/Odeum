#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"
#include "CommandListManager.h"
#include "Colour.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "Buffers/D3DBuffer.h"
#include "DynamicDescriptorHeap.h"
#include "D3DCore.h"
#include <queue>

class ColourBuffer;
class DepthBuffer;
class CommandContext;
class GraphicsContext;
class ComputeContext;

// TODO Aidan: Make comments

/*
The idea of the context is to extrapolate all of the functions needed for setting up and drawing a rendering state.

 This includes:
      --- Initialize
      Set the root signature
      Set the pipeline state
      --- Render
      Clearing various render/dsv targets
      Setting the non scene-specific resources (render targets, dsvs, topology, viewport/scissor rect, etc.)
      Set heaped handles and root const views
      Set vertex/index data
      Draw

 The context also manages the creation of some objects and manages the barriers and fences
  needed for proper work submission/completion and synchronizing between cpu/gpu.

 The code here is the culmination of all the wrappers and managers created. The context simply wraps the specific gpu calls such that it is simpler for the end user
*/


class ContextManager
{
public:

    ContextManager() {}

    CommandContext* AllocateContext(D3D12_COMMAND_LIST_TYPE type_);
    void FreeContext(CommandContext* context_);
    void DestroyAllContexts();

private:

    std::vector<std::unique_ptr<CommandContext>> sm_ContextPool[4];
    std::queue<CommandContext*> sm_availableContexts[4];
    std::mutex sm_contextAllocationMutex;
};

// Base Class for calling commands on the gpu. Base class is graphics/compute agnostic, derived specify procedures for each render type
class CommandContext
{
    friend class ContextManager;

    // Functions used by context manager
    void Reset();
    CommandContext(D3D12_COMMAND_LIST_TYPE type_);

public:

    CommandContext() = default;
    CommandContext(const CommandContext&) = delete;
    CommandContext& operator=(const CommandContext&) = delete;

    ~CommandContext();

    static CommandContext& RequestContext(std::wstring name_ = L"");

    void Initialize();

    GraphicsContext& GetGraphicsContext() {
        ASSERT(m_type != D3D12_COMMAND_LIST_TYPE_COMPUTE, "Cannot convert compute context to graphics context");
        return reinterpret_cast<GraphicsContext&>(*this);
    }

    ComputeContext& GetComputeContext() { return reinterpret_cast<ComputeContext&>(*this); }
    ID3D12GraphicsCommandList* GetCommandList() { return m_commandList; }

    void CopyBuffer(D3DResource& dest_, D3DResource& src_);
    void CopyBufferRegion(D3DResource& dest_, size_t destOffset_, D3DResource& src_, size_t srcOffset_, size_t numBytes_);
    void CopySubresource(D3DResource& dest_, UINT destSubIndex_, D3DResource& src_, UINT srcSubIndex);
    // void CopyCounter(D3DResource& dest_, size_t destOffset_, StructuredBuffer& src_);
    // void ResetCount(StructuredBbuffer& buffer_, uint32_t value_ = 0);

    static void InitializeTexture(D3DResource& dest_, UINT numSubresources_, D3D12_SUBRESOURCE_DATA subResource_[]);
    static void InitializeBuffer(D3DResource& dest_, const void* pData_, size_t numBytes_, size_t offset_ = 0);
    static void InitializeTextureArraySlice(D3DResource& dest_, UINT sliceIndex_, D3DResource& src_);
    // static void ReadbackTexture2D(D3DResource& readBackBuffer_, PixelBuffer& srcBuffer_);

    void WriteBuffer(D3DResource& dest_, size_t destOffset_, const void* pData_, size_t numBytes_);
    // void FillBuffer(D3DResource& dest_, size_t destOffset_, float value_, size_t numBytes_);

    void TransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow = false);
    void BeginTransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow = false);
    void InsertUAVBarrier(D3DResource& resource_, bool flushNow = false);
    void InsertAliasBuffer(D3DResource& before_, D3DResource after_, bool flushNow = false);
    inline void FlushResourceBarriers();

    void SetPipelineState(const PSO& pso_);
    void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_);
    void SetDescriptorHeaps(UINT heapCount_, D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heapPtrs_[]);

protected:

    void BindDescriptorHeaps();

    CommandListManager* m_manager;
    ID3D12GraphicsCommandList* m_commandList;
    ID3D12CommandAllocator* m_currentAllocator;

    ID3D12RootSignature* m_curGraphicsRootSignature;
    ID3D12PipelineState* m_curPipelineState;
    ID3D12RootSignature* m_curComputeRootSignature;

    DynamicDescriptorHeap m_dynamicViewDescHeap;
    DynamicDescriptorHeap m_dynamicSamplerDescHeap;

    D3D12_RESOURCE_BARRIER m_barrierBuffer[16];
    UINT m_numBarriersToFlush;

    ID3D12DescriptorHeap* m_currentDescHeap[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    std::wstring m_id;

    D3D12_COMMAND_LIST_TYPE m_type;
};

class GraphicsContext : public CommandContext
{
public:

    static GraphicsContext& RequestContext(const std::wstring& ID = L"")
    {
        return CommandContext::RequestContext(ID).GetGraphicsContext();
    }

    // Clear buffers for draw
    void ClearUAV(D3DResource& target_);
    void ClearUAV(ColourBuffer& target_);
    void ClearColor(ColourBuffer& target_);
    void ClearDepth(DepthBuffer& target_);
    void ClearStencil(DepthBuffer& target_);
    void ClearDepthAndStencil(DepthBuffer& target_);

    // Set base root signature
    void SetRootSignature(const RootSignature& rootSig_);

    // Set render target
    void SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[]);
    void SetRenderTargets(UINT numRTVs_, const D3D12_CPU_DESCRIPTOR_HANDLE rtvs_[], D3D12_CPU_DESCRIPTOR_HANDLE dsv_);
    void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv_) { SetRenderTargets(1, &rtv_); }
    void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtv_, D3D12_CPU_DESCRIPTOR_HANDLE dsv_) { SetRenderTargets(1, &rtv_, dsv_); }
    void SetDepthStencilTarget(D3D12_CPU_DESCRIPTOR_HANDLE dsv_) { SetRenderTargets(0, nullptr, dsv_); }

    // Set viewport and drawing settings
    void SetViewport(const D3D12_VIEWPORT& viewport_);
    void SetViewport(FLOAT x_, FLOAT y_, FLOAT w_, FLOAT h_, FLOAT minDepth_ = 0.0f, FLOAT maxDepth_ = 1.0f);
    void SetScissor(const D3D12_RECT& rect_);
    void SetScissor(UINT left_, UINT top_, UINT right_, UINT bottom_);
    void SetViewportAndScissor(const D3D12_VIEWPORT& viewport_, const D3D12_RECT& rect_);
    void SetViewportAndScissor(UINT x_, UINT y_, UINT w_, UINT h_);
    void SetStencilRef(UINT stencilRef_);
    void SetBlendFactor(Colour blend_);
    void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology_);

    // Set data into root signature
    void SetConstantArray(UINT rootIndex_, UINT numConstants_, const void* pConstants);
    void SetConstant(UINT rootIndex_, UINT val_, UINT Offset = 0);
    void SetConstants(UINT rootIndex_, UINT x_);
    void SetConstants(UINT rootIndex_, UINT x_, UINT y_);
    void SetConstants(UINT rootIndex_, UINT x_, UINT y_, UINT z_);
    void SetConstants(UINT rootIndex_, UINT x_, UINT y_, UINT z_, UINT w_);
    void SetConstantBuffer(UINT rootIndex_, D3D12_GPU_VIRTUAL_ADDRESS cbv_);
    void SetDynamicConstantBufferView(UINT rootIndex_, size_t bufferSize_, const void* cBufferData_);
    void SetBufferSRV(UINT rootIndex_, const D3DResource& srv_, UINT64 offset_ = 0);
    void SetBufferUAV(UINT rootIndex_, const D3DResource& uav_, UINT64 offset_ = 0);
    void SetDescriptorTable(UINT rootIndex_, D3D12_GPU_DESCRIPTOR_HANDLE handle_);

    // Set Dynamic descriptors and samplers from heaps
    void SetDynamicDescriptor(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_);
    void SetDynamicDescriptors(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[]);
    void SetDynamicSampler(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_);
    void SetDynamicSamplers(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[]);

    // Setting our primary buffers
    void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBufferView_);
    void SetVertexBuffer(UINT slot_, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView_);
    void SetVertexBuffers(UINT startSlot_, UINT count_, const D3D12_VERTEX_BUFFER_VIEW vertexBufferViews_[]);
    void SetDynamicVB(UINT slot_, size_t numVerts_, size_t vertStride_, const void* vertexBufferData_);
    void SetDynamicIB(size_t indexCount_, const uint16_t* indexBufferData_);
    void SetDynamicSRV(UINT rootIndex_, size_t bufferSize_, const void* srvBufferData_);

    // Different kinds of drawing
    void Draw(UINT vertexCount_, UINT vertexStartOffset_ = 0);
    void DrawIndexed(UINT indexCount_, UINT startIndexLocation_ = 0, int baseVertexLocation_ = 0);
    void DrawInstanced(UINT vertexCountPerInstance_, UINT instanceCount_,
        UINT startVertexLocation_ = 0, UINT startInstanceLocation_ = 0);
    void DrawIndexedInstanced(UINT indexCountPerInstance_, UINT instanceCount_, UINT startIndexLocation_,
        int baseVertexLocation_, UINT startInstanceLocation_);
};

class ComputeContext : public CommandContext
{
};

inline void CommandContext::FlushResourceBarriers()
{
    if (m_numBarriersToFlush > 0)
    {
        m_commandList->ResourceBarrier(m_numBarriersToFlush, m_barrierBuffer);
        m_numBarriersToFlush = 0;
    }
}

inline void CommandContext::SetPipelineState(const PSO& pso_)
{

}

inline void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_)
{
}

inline void CommandContext::SetDescriptorHeaps(UINT heapCount_, D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heapPtrs_[])
{
}

inline void CommandContext::CopyBuffer(D3DResource& dest_, D3DResource& src_)
{
}

inline void CommandContext::CopyBufferRegion(D3DResource& dest_, size_t destOffset_, D3DResource& src_, size_t srcOffset_, size_t numBytes_)
{
}

inline void GraphicsContext::SetRootSignature(const RootSignature& rootSig_)
{
}

inline void GraphicsContext::SetScissor(UINT left_, UINT top_, UINT right_, UINT bottom_)
{
}

inline void GraphicsContext::SetViewportAndScissor(UINT x_, UINT y_, UINT w_, UINT h_)
{
}

inline void GraphicsContext::SetStencilRef(UINT stencilRef_)
{
}

inline void GraphicsContext::SetBlendFactor(Colour blend_)
{
}

inline void GraphicsContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology_)
{
}

inline void GraphicsContext::SetConstantArray(UINT rootIndex_, UINT numConstants_, const void* pConstants)
{
}

inline void GraphicsContext::SetConstant(UINT rootIndex_, UINT val_, UINT Offset)
{
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, UINT x_)
{
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, UINT x_, UINT y_)
{
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, UINT x_, UINT y_, UINT z_)
{
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, UINT x_, UINT y_, UINT z_, UINT w_)
{
}

inline void GraphicsContext::SetConstantBuffer(UINT rootIndex_, D3D12_GPU_VIRTUAL_ADDRESS cbv_)
{
}

inline void GraphicsContext::SetDynamicConstantBufferView(UINT rootIndex_, size_t bufferSize_, const void* cBufferData_)
{
}

inline void GraphicsContext::SetDynamicVB(UINT slot_, size_t numVerts_, size_t vertStride_, const void* vertexBufferData_)
{
}

inline void GraphicsContext::SetDynamicIB(size_t indexCount_, const uint16_t* indexBufferData_)
{
}

inline void GraphicsContext::SetBufferSRV(UINT rootIndex_, const D3DResource& srv_, UINT64 offset_)
{
}

inline void GraphicsContext::SetBufferUAV(UINT rootIndex_, const D3DResource& uav_, UINT64 offset_)
{
}

inline void GraphicsContext::SetDescriptorTable(UINT rootIndex_, D3D12_GPU_DESCRIPTOR_HANDLE handle_)
{
}

inline void GraphicsContext::SetDynamicDescriptor(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_)
{
}

inline void GraphicsContext::SetDynamicDescriptors(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
{
}

inline void GraphicsContext::SetDynamicSampler(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_)
{
}

inline void GraphicsContext::SetDynamicSamplers(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
{
}

inline void GraphicsContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBufferView_)
{
}

inline void GraphicsContext::SetVertexBuffer(UINT slot_, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView_)
{
}

inline void GraphicsContext::SetVertexBuffers(UINT startSlot_, UINT count_, const D3D12_VERTEX_BUFFER_VIEW vertexBufferViews_[])
{
}

inline void GraphicsContext::Draw(UINT vertexCount_, UINT vertexStartOffset_)
{
}

inline void GraphicsContext::DrawIndexed(UINT indexCount_, UINT startIndexLocation_, int baseVertexLocation_)
{
}

inline void GraphicsContext::DrawInstanced(UINT vertexCountPerInstance_, UINT instanceCount_, UINT startVertexLocation_, UINT startInstanceLocation_)
{
}

inline void GraphicsContext::DrawIndexedInstanced(UINT indexCountPerInstance_, UINT instanceCount_, UINT startIndexLocation_, int baseVertexLocation_, UINT startInstanceLocation_)
{
}

#endif