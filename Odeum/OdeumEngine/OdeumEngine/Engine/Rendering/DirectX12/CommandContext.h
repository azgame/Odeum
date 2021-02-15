// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 

#ifndef COMMANDCONTEXT_H
#define COMMANDCONTEXT_H

#include "D3DIncludes.h"
#include "CommandListManager.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "Buffers/D3DBuffer.h"
#include "DynamicDescriptorHeap.h"
#include "BufferAllocator.h"
#include "D3DCore.h"
#include "Colour.h"
#include "CommandSignature.h"
#include <queue>

class ColourBuffer;
class DepthBuffer;
class PixelBuffer;
class CommandContext;
class GraphicsContext;
class ComputeContext;

// Comments by Aidan Zizys, 2020

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

struct DWParam
{
    DWParam(FLOAT f) : Float(f) {}
    DWParam(UINT u) : Uint(u) {}
    DWParam(INT i) : Int(i) {}

    void operator= (FLOAT f) { Float = f; }
    void operator= (UINT u) { Uint = u; }
    void operator= (INT i) { Int = i; }

    union
    {
        FLOAT Float;
        UINT Uint;
        INT Int;
    };
};

#define VALID_COMPUTE_QUEUE_RESOURCE_STATES \
    ( D3D12_RESOURCE_STATE_UNORDERED_ACCESS \
    | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE \
    | D3D12_RESOURCE_STATE_COPY_DEST \
    | D3D12_RESOURCE_STATE_COPY_SOURCE )

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

// Base Class for calling commands on the gpu. Base class is graphics/compute agnostic, derived specify procedures for each "render" type
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

    static void DestroyAllContexts();

    static CommandContext& RequestContext(std::wstring name_ = L"");

    uint64_t Flush(bool waitForCompletion_ = false);

    uint64_t Finish(bool waitForCompletion_ = false);

    void Initialize();

    GraphicsContext& GetGraphicsContext() {
        ASSERT(m_type != D3D12_COMMAND_LIST_TYPE_COMPUTE, "Cannot convert compute context to graphics context");
        return reinterpret_cast<GraphicsContext&>(*this);
    }

    BufferEntry ReserveBufferMemory(size_t byteSize_)
    {
        return m_CpuBufferAllocator.Allocate(byteSize_);
    }

    ComputeContext& GetComputeContext() { return reinterpret_cast<ComputeContext&>(*this); }
    ID3D12GraphicsCommandList* GetCommandList() { return m_commandList; }

    void CopyBuffer(D3DResource& dest_, D3DResource& src_);
    void CopyBufferRegion(D3DResource& dest_, size_t destOffset_, D3DResource& src_, size_t srcOffset_, size_t numBytes_);
    void CopySubresource(D3DResource& dest_, UINT destSubIndex_, D3DResource& src_, UINT srcSubIndex);
    void CopyCounter(D3DResource& dest_, size_t destOffset_, StructuredBuffer& src_);
    void ResetCounter(StructuredBuffer& buffer_, uint32_t value_ = 0);

    static void InitializeTexture(D3DResource& dest_, UINT numSubresources_, D3D12_SUBRESOURCE_DATA subResource_[]);
    static void InitializeBuffer(D3DResource& dest_, const void* pData_, size_t numBytes_, size_t offset_ = 0);
    static void InitializeTextureArraySlice(D3DResource& dest_, UINT sliceIndex_, D3DResource& src_);
    static void ReadbackTexture2D(D3DResource& readBackBuffer_, PixelBuffer& srcBuffer_);

    void WriteBuffer(D3DResource& dest_, size_t destOffset_, const void* pData_, size_t numBytes_);
    void FillBuffer(D3DResource& dest_, size_t destOffset_, float value_, size_t numBytes_);

    void TransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow = false);
    void BeginTransitionResource(D3DResource& resource_, D3D12_RESOURCE_STATES newState_, bool flushNow = false);
    void InsertUAVBarrier(D3DResource& resource_, bool flushNow = false);
    void InsertAliasBuffer(D3DResource& before_, D3DResource after_, bool flushNow = false);
    void FlushResourceBarriers();

    void SetPipelineState(const PSO& pso_);
    void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_);
    void SetDescriptorHeaps(UINT heapCount_, D3D12_DESCRIPTOR_HEAP_TYPE type_[], ID3D12DescriptorHeap* heapPtrs_[]);

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

    ID3D12DescriptorHeap* m_currentDescHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

    BufferAllocator m_CpuBufferAllocator;
    BufferAllocator m_GpuBufferAllocator;

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
    void ClearUAV(D3DBuffer& target_);
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
    void SetConstant(UINT rootIndex_, DWParam val_, UINT Offset = 0);
    void SetConstants(UINT rootIndex_, DWParam x_);
    void SetConstants(UINT rootIndex_, DWParam x_, DWParam y_);
    void SetConstants(UINT rootIndex_, DWParam x_, DWParam y_, DWParam z_);
    void SetConstants(UINT rootIndex_, DWParam x_, DWParam y_, DWParam z_, DWParam w_);
    void SetConstantBuffer(UINT rootIndex_, D3D12_GPU_VIRTUAL_ADDRESS cbv_);
    void SetDynamicConstantBufferView(UINT rootIndex_, size_t bufferSize_, const void* cBufferData_);
    void SetBufferSRV(UINT rootIndex_, const D3DBuffer& srv_, UINT64 offset_ = 0);
    void SetBufferUAV(UINT rootIndex_, const D3DBuffer& uav_, UINT64 offset_ = 0);
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
    void DrawIndirect(D3DBuffer& ArgumentBuffer, uint64_t ArgumentBufferOffset = 0);
    void ExecuteIndirect(CommandSignature& CommandSig, D3DBuffer& ArgumentBuffer, uint64_t ArgumentStartOffset = 0,
        uint32_t MaxCommands = 1, D3DBuffer* CommandCounterBuffer = nullptr, uint64_t CounterOffset = 0);
};

class ComputeContext : public CommandContext
{
public:
    static ComputeContext& RequestContext(const std::wstring& ID = L"", bool Async = false);

    void ClearUAV(D3DBuffer& Target);
    void ClearUAV(ColourBuffer& Target);

    void SetRootSignature(const RootSignature& RootSig);

    void SetConstantArray(UINT RootIndex, UINT NumConstants, const void* pConstants);
    void SetConstant(UINT RootIndex, DWParam Val, UINT Offset = 0);
    void SetConstants(UINT RootIndex, DWParam X);
    void SetConstants(UINT RootIndex, DWParam X, DWParam Y);
    void SetConstants(UINT RootIndex, DWParam X, DWParam Y, DWParam Z);
    void SetConstants(UINT RootIndex, DWParam X, DWParam Y, DWParam Z, DWParam W);
    void SetConstantBuffer(UINT RootIndex, D3D12_GPU_VIRTUAL_ADDRESS CBV);
    void SetDynamicConstantBufferView(UINT RootIndex, size_t BufferSize, const void* BufferData);
    void SetDynamicSRV(UINT RootIndex, size_t BufferSize, const void* BufferData);
    void SetBufferSRV(UINT RootIndex, const D3DBuffer& SRV, UINT64 Offset = 0);
    void SetBufferUAV(UINT RootIndex, const D3DBuffer& UAV, UINT64 Offset = 0);
    void SetDescriptorTable(UINT RootIndex, D3D12_GPU_DESCRIPTOR_HANDLE FirstHandle);

    void SetDynamicDescriptor(UINT RootIndex, UINT Offset, D3D12_CPU_DESCRIPTOR_HANDLE Handle);
    void SetDynamicDescriptors(UINT RootIndex, UINT Offset, UINT Count, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[]);
    void SetDynamicSampler(UINT RootIndex, UINT Offset, D3D12_CPU_DESCRIPTOR_HANDLE Handle);
    void SetDynamicSamplers(UINT RootIndex, UINT Offset, UINT Count, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[]);

    void Dispatch(size_t GroupCountX = 1, size_t GroupCountY = 1, size_t GroupCountZ = 1);
    void Dispatch1D(size_t ThreadCountX, size_t GroupSizeX = 64);
    void Dispatch2D(size_t ThreadCountX, size_t ThreadCountY, size_t GroupSizeX = 8, size_t GroupSizeY = 8);
    void Dispatch3D(size_t ThreadCountX, size_t ThreadCountY, size_t ThreadCountZ, size_t GroupSizeX, size_t GroupSizeY, size_t GroupSizeZ);
    void DispatchIndirect(D3DBuffer& ArgumentBuffer, uint64_t ArgumentBufferOffset = 0);
    void ExecuteIndirect(CommandSignature& CommandSig, D3DBuffer& ArgumentBuffer, uint64_t ArgumentStartOffset = 0,
        uint32_t MaxCommands = 1, D3DBuffer* CommandCounterBuffer = nullptr, uint64_t CounterOffset = 0);
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
    ID3D12PipelineState* pipelineState = pso_.GetPipelineState();
    if (pipelineState == m_curPipelineState)
        return;

    m_commandList->SetPipelineState(pipelineState);
    m_curPipelineState = pipelineState;
}

inline void CommandContext::SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_)
{
    if (m_currentDescHeaps[type_] != heap_)
    {
        m_currentDescHeaps[type_] = heap_;
        BindDescriptorHeaps();
    }
}

inline void CommandContext::SetDescriptorHeaps(UINT heapCount_, D3D12_DESCRIPTOR_HEAP_TYPE type_[], ID3D12DescriptorHeap* heapPtrs_[])
{
    bool updatedHeaps = false;

    for (UINT i = 0; i < heapCount_; i++)
    {
        if (m_currentDescHeaps[type_[i]] != heapPtrs_[i])
        {
            m_currentDescHeaps[type_[i]] = heapPtrs_[i];
            updatedHeaps = true;
        }
    }

    if (updatedHeaps)
        BindDescriptorHeaps();
}

inline void CommandContext::CopyBuffer(D3DResource& dest_, D3DResource& src_)
{
    TransitionResource(dest_, D3D12_RESOURCE_STATE_COPY_DEST);
    TransitionResource(src_, D3D12_RESOURCE_STATE_COPY_SOURCE);
    FlushResourceBarriers();
    m_commandList->CopyResource(dest_.GetResource(), src_.GetResource());
}

inline void CommandContext::CopyBufferRegion(D3DResource& dest_, size_t destOffset_, D3DResource& src_, size_t srcOffset_, size_t numBytes_)
{
    TransitionResource(dest_, D3D12_RESOURCE_STATE_COPY_DEST);
    FlushResourceBarriers();
    m_commandList->CopyBufferRegion(dest_.GetResource(), destOffset_, src_.GetResource(), srcOffset_, numBytes_);
}

inline void GraphicsContext::SetRootSignature(const RootSignature& rootSig_)
{
    if (rootSig_.GetRootSignature() == m_curGraphicsRootSignature)
        return;

    m_commandList->SetGraphicsRootSignature(m_curGraphicsRootSignature = rootSig_.GetRootSignature());

    m_dynamicViewDescHeap.ParseGraphicsRootSignature(rootSig_);
    m_dynamicSamplerDescHeap.ParseGraphicsRootSignature(rootSig_);
}

inline void ComputeContext::SetRootSignature(const RootSignature& rootSig)
{
    if (rootSig.GetRootSignature() == m_curComputeRootSignature)
        return;

    m_commandList->SetComputeRootSignature(m_curComputeRootSignature = rootSig.GetRootSignature());

    m_dynamicViewDescHeap.ParseComputeRootSignature(rootSig);
    m_dynamicSamplerDescHeap.ParseComputeRootSignature(rootSig);
}

inline void GraphicsContext::SetScissor(UINT left_, UINT top_, UINT right_, UINT bottom_)
{
    SetScissor(CD3DX12_RECT(left_, top_, right_, bottom_));
}

inline void GraphicsContext::SetViewportAndScissor(UINT x_, UINT y_, UINT w_, UINT h_)
{
    SetViewport((float)x_, (float)y_, (float)w_, (float)h_);
    SetScissor(x_, y_, x_ + w_, y_ + h_);
}

inline void GraphicsContext::SetStencilRef(UINT stencilRef_)
{
    m_commandList->OMSetStencilRef(stencilRef_);
}

inline void GraphicsContext::SetBlendFactor(Colour blend_)
{
    m_commandList->OMSetBlendFactor(blend_.GetPtr());
}

inline void GraphicsContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology_)
{
    m_commandList->IASetPrimitiveTopology(topology_);
}

inline void ComputeContext::SetConstantArray(UINT RootEntry, UINT NumConstants, const void* pConstants)
{
    m_commandList->SetComputeRoot32BitConstants(RootEntry, NumConstants, pConstants, 0);
}

inline void ComputeContext::SetConstant(UINT RootEntry, DWParam Val, UINT Offset)
{
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Val.Uint, Offset);
}

inline void ComputeContext::SetConstants(UINT RootEntry, DWParam X)
{
    m_commandList->SetComputeRoot32BitConstant(RootEntry, X.Uint, 0);
}

inline void ComputeContext::SetConstants(UINT RootEntry, DWParam X, DWParam Y)
{
    m_commandList->SetComputeRoot32BitConstant(RootEntry, X.Uint, 0);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Y.Uint, 1);
}

inline void ComputeContext::SetConstants(UINT RootEntry, DWParam X, DWParam Y, DWParam Z)
{
    m_commandList->SetComputeRoot32BitConstant(RootEntry, X.Uint, 0);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Y.Uint, 1);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Z.Uint, 2);
}

inline void ComputeContext::SetConstants(UINT RootEntry, DWParam X, DWParam Y, DWParam Z, DWParam W)
{
    m_commandList->SetComputeRoot32BitConstant(RootEntry, X.Uint, 0);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Y.Uint, 1);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, Z.Uint, 2);
    m_commandList->SetComputeRoot32BitConstant(RootEntry, W.Uint, 3);
}

inline void GraphicsContext::SetConstantArray(UINT rootIndex_, UINT numConstants_, const void* pConstants_)
{
    m_commandList->SetGraphicsRoot32BitConstants(rootIndex_, numConstants_, pConstants_, 0);
}

inline void GraphicsContext::SetConstant(UINT rootIndex_, DWParam val_, UINT offset_)
{
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, val_.Uint, offset_);
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, DWParam x_)
{
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, x_.Uint, 0);
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, DWParam x_, DWParam y_)
{
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, x_.Uint, 0);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, y_.Uint, 1);
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, DWParam x_, DWParam y_, DWParam z_)
{
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, x_.Uint, 0);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, y_.Uint, 1);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, z_.Uint, 2);
}

inline void GraphicsContext::SetConstants(UINT rootIndex_, DWParam x_, DWParam y_, DWParam z_, DWParam w_)
{
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, x_.Uint, 0);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, y_.Uint, 1);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, z_.Uint, 2);
    m_commandList->SetGraphicsRoot32BitConstant(rootIndex_, w_.Uint, 3);
}

inline void ComputeContext::SetConstantBuffer(UINT RootIndex, D3D12_GPU_VIRTUAL_ADDRESS CBV)
{
    m_commandList->SetComputeRootConstantBufferView(RootIndex, CBV);
}

inline void GraphicsContext::SetConstantBuffer(UINT rootIndex_, D3D12_GPU_VIRTUAL_ADDRESS cbv_)
{
    m_commandList->SetGraphicsRootConstantBufferView(rootIndex_, cbv_);
}

inline void GraphicsContext::SetDynamicConstantBufferView(UINT rootIndex_, size_t bufferSize_, const void* cBufferData_)
{
    ASSERT(cBufferData_ != nullptr && Utility::isAligned(cBufferData_, 16));
    BufferEntry constantBuffer = m_CpuBufferAllocator.Allocate(bufferSize_);
    memcpy(constantBuffer.CpuAddress, cBufferData_, bufferSize_);
    m_commandList->SetGraphicsRootConstantBufferView(rootIndex_, constantBuffer.GpuAddress);
}

inline void ComputeContext::SetDynamicConstantBufferView(UINT RootIndex, size_t BufferSize, const void* BufferData)
{
    ASSERT(BufferData != nullptr && Utility::isAligned(BufferData, 16));
    BufferEntry constantBuffer = m_CpuBufferAllocator.Allocate(BufferSize);
    memcpy(constantBuffer.CpuAddress, BufferData, BufferSize);
    m_commandList->SetComputeRootConstantBufferView(RootIndex, constantBuffer.GpuAddress);
}

inline void GraphicsContext::SetDynamicVB(UINT slot_, size_t numVerts_, size_t vertStride_, const void* vertexBufferData_)
{
    ASSERT(vertexBufferData_ != nullptr && Utility::isAligned(vertexBufferData_, 16));
    size_t bufferSize = numVerts_ * vertStride_;
    bufferSize = Utility::Align(bufferSize, 16);

    BufferEntry vertexBuffer = m_CpuBufferAllocator.Allocate(bufferSize);

    memcpy(vertexBuffer.CpuAddress, vertexBufferData_, bufferSize);

    D3D12_VERTEX_BUFFER_VIEW vbView;
    vbView.BufferLocation = vertexBuffer.GpuAddress;
    vbView.SizeInBytes = (UINT)bufferSize;
    vbView.StrideInBytes = (UINT)vertStride_;

    m_commandList->IASetVertexBuffers(slot_, 1, &vbView);
}

inline void GraphicsContext::SetDynamicIB(size_t indexCount_, const uint16_t* indexBufferData_)
{
    ASSERT(indexBufferData_ != nullptr && Utility::isAligned(indexBufferData_, 16));
    size_t bufferSize = indexCount_ * sizeof(uint16_t);
    bufferSize = Utility::Align(bufferSize, 16);

    BufferEntry indexBuffer = m_CpuBufferAllocator.Allocate(bufferSize);

    memcpy(indexBuffer.CpuAddress, indexBufferData_, bufferSize);

    D3D12_INDEX_BUFFER_VIEW ibView;
    ibView.BufferLocation = indexBuffer.GpuAddress;
    ibView.SizeInBytes = (UINT)(indexCount_ * sizeof(uint16_t));
    ibView.Format = DXGI_FORMAT_R16_UINT;

    m_commandList->IASetIndexBuffer(&ibView);
}

inline void GraphicsContext::SetDynamicSRV(UINT rootIndex_, size_t bufferSize_, const void* srvBufferData_)
{
    ASSERT(srvBufferData_ != nullptr && Utility::isAligned(srvBufferData_, 16));
    BufferEntry buffer = m_CpuBufferAllocator.Allocate(bufferSize_);
    bufferSize_ = Utility::Align(bufferSize_, 16);
    memcpy(buffer.CpuAddress, srvBufferData_, bufferSize_);
    m_commandList->SetGraphicsRootShaderResourceView(rootIndex_, buffer.GpuAddress);
}

inline void ComputeContext::SetDynamicSRV(UINT RootIndex, size_t BufferSize, const void* BufferData)
{
    ASSERT(BufferData != nullptr && Utility::isAligned(BufferData, 16));
    BufferEntry srvBuffer = m_CpuBufferAllocator.Allocate(BufferSize);
    memcpy(srvBuffer.CpuAddress, BufferData, BufferSize);
    m_commandList->SetComputeRootShaderResourceView(RootIndex, srvBuffer.GpuAddress);
}

inline void GraphicsContext::SetBufferSRV(UINT rootIndex_, const D3DBuffer& srv_, UINT64 offset_)
{
    ASSERT((srv_.currentState & D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE) != 0);
    m_commandList->SetGraphicsRootShaderResourceView(rootIndex_, srv_.GetGpuVirtualAddress() + offset_);
}

inline void ComputeContext::SetBufferSRV(UINT RootIndex, const D3DBuffer& SRV, UINT64 Offset)
{
    ASSERT((SRV.currentState & D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE) != 0);
    m_commandList->SetComputeRootShaderResourceView(RootIndex, SRV.GetGpuVirtualAddress() + Offset);
}

inline void GraphicsContext::SetBufferUAV(UINT rootIndex_, const D3DBuffer& uav_, UINT64 offset_)
{
    ASSERT((uav_.currentState & D3D12_RESOURCE_STATE_UNORDERED_ACCESS) != 0);
    m_commandList->SetGraphicsRootUnorderedAccessView(rootIndex_, uav_.GetGpuVirtualAddress() + offset_);
}

inline void ComputeContext::SetBufferUAV(UINT RootIndex, const D3DBuffer& UAV, UINT64 Offset)
{
    ASSERT((UAV.currentState & D3D12_RESOURCE_STATE_UNORDERED_ACCESS) != 0);
    m_commandList->SetComputeRootUnorderedAccessView(RootIndex, UAV.GetGpuVirtualAddress() + Offset);
}

inline void ComputeContext::Dispatch(size_t GroupCountX, size_t GroupCountY, size_t GroupCountZ)
{
    FlushResourceBarriers();
    m_dynamicViewDescHeap.CommitComputeDescriptorTables(m_commandList);
    m_dynamicSamplerDescHeap.CommitComputeDescriptorTables(m_commandList);
    m_commandList->Dispatch((UINT)GroupCountX, (UINT)GroupCountY, (UINT)GroupCountZ);
}

inline void ComputeContext::Dispatch1D(size_t ThreadCountX, size_t GroupSizeX)
{
    Dispatch(Utility::AlignedDivide(ThreadCountX, GroupSizeX), 1, 1);
}

inline void ComputeContext::Dispatch2D(size_t ThreadCountX, size_t ThreadCountY, size_t GroupSizeX, size_t GroupSizeY)
{
    Dispatch(
        Utility::AlignedDivide(ThreadCountX, GroupSizeX),
        Utility::AlignedDivide(ThreadCountY, GroupSizeY), 1);
}

inline void ComputeContext::Dispatch3D(size_t ThreadCountX, size_t ThreadCountY, size_t ThreadCountZ, size_t GroupSizeX, size_t GroupSizeY, size_t GroupSizeZ)
{
    Dispatch(
        Utility::AlignedDivide(ThreadCountX, GroupSizeX),
        Utility::AlignedDivide(ThreadCountY, GroupSizeY),
        Utility::AlignedDivide(ThreadCountZ, GroupSizeZ));
}

inline void GraphicsContext::SetDescriptorTable(UINT rootIndex_, D3D12_GPU_DESCRIPTOR_HANDLE handle_)
{
    m_commandList->SetGraphicsRootDescriptorTable(rootIndex_, handle_);
}

inline void ComputeContext::SetDescriptorTable(UINT RootIndex, D3D12_GPU_DESCRIPTOR_HANDLE FirstHandle)
{
    m_commandList->SetComputeRootDescriptorTable(RootIndex, FirstHandle);
}

inline void GraphicsContext::SetDynamicDescriptor(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_)
{
    SetDynamicDescriptors(rootIndex_, offset_, 1, &handle_);
}

inline void ComputeContext::SetDynamicDescriptor(UINT RootIndex, UINT Offset, D3D12_CPU_DESCRIPTOR_HANDLE Handle)
{
    SetDynamicDescriptors(RootIndex, Offset, 1, &Handle);
}

inline void GraphicsContext::SetDynamicDescriptors(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
{
    m_dynamicViewDescHeap.SetGraphicsDescriptorHandle(rootIndex_, offset_, count_, handles_);
}

inline void ComputeContext::SetDynamicDescriptors(UINT RootIndex, UINT Offset, UINT Count, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
{
    m_dynamicViewDescHeap.SetComputeDescriptorHandles(RootIndex, Offset, Count, Handles);
}

inline void GraphicsContext::SetDynamicSampler(UINT rootIndex_, UINT offset_, D3D12_CPU_DESCRIPTOR_HANDLE handle_)
{
    SetDynamicSamplers(rootIndex_, offset_, 1, &handle_);
}

inline void ComputeContext::SetDynamicSampler(UINT RootIndex, UINT Offset, D3D12_CPU_DESCRIPTOR_HANDLE Handle)
{
    SetDynamicSamplers(RootIndex, Offset, 1, &Handle);
}

inline void ComputeContext::SetDynamicSamplers(UINT RootIndex, UINT Offset, UINT Count, const D3D12_CPU_DESCRIPTOR_HANDLE Handles[])
{
    m_dynamicSamplerDescHeap.SetComputeDescriptorHandles(RootIndex, Offset, Count, Handles);
}

inline void GraphicsContext::SetDynamicSamplers(UINT rootIndex_, UINT offset_, UINT count_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
{
    m_dynamicSamplerDescHeap.SetGraphicsDescriptorHandle(rootIndex_, offset_, count_, handles_);
}

inline void GraphicsContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBufferView_)
{
    m_commandList->IASetIndexBuffer(&indexBufferView_);
}

inline void GraphicsContext::SetVertexBuffer(UINT slot_, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView_)
{
    SetVertexBuffers(slot_, 1, &vertexBufferView_);
}

inline void GraphicsContext::SetVertexBuffers(UINT startSlot_, UINT count_, const D3D12_VERTEX_BUFFER_VIEW vertexBufferViews_[])
{
    m_commandList->IASetVertexBuffers(startSlot_, count_, vertexBufferViews_);
}

inline void GraphicsContext::Draw(UINT vertexCount_, UINT vertexStartOffset_)
{
    DrawInstanced(vertexCount_, 1, vertexStartOffset_, 0);
}

inline void GraphicsContext::DrawIndexed(UINT indexCount_, UINT startIndexLocation_, int baseVertexLocation_)
{
    DrawIndexedInstanced(indexCount_, 1, startIndexLocation_, baseVertexLocation_, 0);
}

inline void GraphicsContext::DrawInstanced(UINT vertexCountPerInstance_, UINT instanceCount_, UINT startVertexLocation_, UINT startInstanceLocation_)
{
    FlushResourceBarriers();
    m_dynamicViewDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_dynamicSamplerDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_commandList->DrawInstanced(vertexCountPerInstance_, instanceCount_, startInstanceLocation_, startInstanceLocation_);
}

inline void GraphicsContext::DrawIndexedInstanced(UINT indexCountPerInstance_, UINT instanceCount_, UINT startIndexLocation_, int baseVertexLocation_, UINT startInstanceLocation_)
{
    FlushResourceBarriers();
    m_dynamicViewDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_dynamicSamplerDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_commandList->DrawIndexedInstanced(indexCountPerInstance_, instanceCount_, startIndexLocation_, baseVertexLocation_, startInstanceLocation_);
}

inline void GraphicsContext::ExecuteIndirect(CommandSignature& CommandSig,
    D3DBuffer& ArgumentBuffer, uint64_t ArgumentStartOffset,
    uint32_t MaxCommands, D3DBuffer* CommandCounterBuffer, uint64_t CounterOffset)
{
    FlushResourceBarriers();
    m_dynamicViewDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_dynamicSamplerDescHeap.CommitGraphicsDescriptorTables(m_commandList);
    m_commandList->ExecuteIndirect(CommandSig.GetSignature(), MaxCommands,
        ArgumentBuffer.GetResource(), ArgumentStartOffset,
        CommandCounterBuffer == nullptr ? nullptr : CommandCounterBuffer->GetResource(), CounterOffset);
}

inline void GraphicsContext::DrawIndirect(D3DBuffer& ArgumentBuffer, uint64_t ArgumentBufferOffset)
{
    ExecuteIndirect(DXGraphics::DrawIndirectCommandSignature, ArgumentBuffer, ArgumentBufferOffset);
}

inline void ComputeContext::ExecuteIndirect(CommandSignature& CommandSig,
    D3DBuffer& ArgumentBuffer, uint64_t ArgumentStartOffset,
    uint32_t MaxCommands, D3DBuffer* CommandCounterBuffer, uint64_t CounterOffset)
{
    FlushResourceBarriers();
    m_dynamicViewDescHeap.CommitComputeDescriptorTables(m_commandList);
    m_dynamicSamplerDescHeap.CommitComputeDescriptorTables(m_commandList);
    m_commandList->ExecuteIndirect(CommandSig.GetSignature(), MaxCommands,
        ArgumentBuffer.GetResource(), ArgumentStartOffset,
        CommandCounterBuffer == nullptr ? nullptr : CommandCounterBuffer->GetResource(), CounterOffset);
}

inline void ComputeContext::DispatchIndirect(D3DBuffer& ArgumentBuffer, uint64_t ArgumentBufferOffset)
{
    ExecuteIndirect(DXGraphics::DispatchIndirectCommandSignature, ArgumentBuffer, ArgumentBufferOffset);
}

inline void CommandContext::CopyCounter(D3DResource& Dest, size_t DestOffset, StructuredBuffer& Src)
{
    TransitionResource(Dest, D3D12_RESOURCE_STATE_COPY_DEST);
    TransitionResource(Src.GetCounterBuffer(), D3D12_RESOURCE_STATE_COPY_SOURCE);
    FlushResourceBarriers();
    m_commandList->CopyBufferRegion(Dest.GetResource(), DestOffset, Src.GetCounterBuffer().GetResource(), 0, 4);
}

inline void CommandContext::ResetCounter(StructuredBuffer& Buf, uint32_t Value)
{
    FillBuffer(Buf.GetCounterBuffer(), 0, (float)Value, sizeof(uint32_t));
    TransitionResource(Buf.GetCounterBuffer(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

#endif