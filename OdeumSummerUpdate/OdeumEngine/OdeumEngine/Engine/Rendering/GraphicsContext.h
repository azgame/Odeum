#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"

#include <queue>

class GraphicsContext;

class ContextManager
{
public:

    ContextManager() {}

    GraphicsContext* AllocateContext(D3D12_COMMAND_LIST_TYPE type_);
    void FreeContext(GraphicsContext* context_);
    void DestroyAllContexts();

private:

    std::vector<std::unique_ptr<GraphicsContext>> sm_ContextPool[4];
    std::queue<GraphicsContext*> sm_availableContexts[4];
    std::mutex sm_contextAllocationMutex;
};

class GraphicsContext
{
    friend class ContextManager;

    // Functions used by context manager
    void Reset();
    GraphicsContext(D3D12_COMMAND_LIST_TYPE type_) : m_type(type_) {}

public:

	static GraphicsContext* Initialize(std::string name_ = "");

	void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_);

	GraphicsContext() {}
	~GraphicsContext();

private:

    D3D12_COMMAND_LIST_TYPE m_type;
};

#endif