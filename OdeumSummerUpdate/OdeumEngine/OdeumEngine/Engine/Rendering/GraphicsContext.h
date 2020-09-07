#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"
#include "CommandListManager.h"
#include "Colour.h"
#include "PipelineState.h"
#include "RootSignature.h"
#include "Buffers/D3DBuffer.h"
#include "Buffers/PixelBuffer.h"
#include "DynamicDescriptorHeap.h"
#include "D3DCore.h"
#include <queue>

class CommandContext;
class GraphicsContext;
class ComputeContext;

// TODO Aidan: Make comments

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

    static CommandContext& Initialize(std::wstring name_ = L"");

    GraphicsContext& GetGraphicsContext() {
        assert(m_type != D3D12_COMMAND_LIST_TYPE_COMPUTE, "Cannot convert async compute context to graphics");
        return reinterpret_cast<GraphicsContext&>(*this);
    }

    ComputeContext& GetComputeContext() {
        return reinterpret_cast<ComputeContext&>(*this);
    }

    ID3D12GraphicsCommandList* GetCommandList() {
        return m_commandList;
    }

    void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_);

protected:

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

	

    static GraphicsContext& Initialize(const std::wstring& ID = L"")
    {
        return CommandContext::Initialize(ID).GetGraphicsContext();
    }

private:
};

class ComputeContext : public CommandContext
{

};

#endif