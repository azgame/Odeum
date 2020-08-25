#ifndef COMMANDLISTMANAGER_H
#define COMMANDLISTMANAGER_H

#include "../../pch.h"
#include <stdint.h>
#include "CommandAllocatorPool.h"

class CommandQueue
{
	friend class CommandListManager;
	friend class GraphicsContext;

public:

	CommandQueue(D3D12_COMMAND_LIST_TYPE type_);
	~CommandQueue();

	void Initialize(ID3D12Device* device_);
	void Uninitialize();

	inline bool isReady() { return m_commandQueue != nullptr; }

private:

	uint64_t ExecuteCommandList(ID3D12CommandList* cmdList_);
	ID3D12CommandAllocator* RequestAllocator();
	void DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_);

	ID3D12CommandQueue* m_commandQueue;
	const D3D12_COMMAND_LIST_TYPE m_type;
	CommandAllocatorPool m_allocatorPool;
	std::mutex m_fenceMutex;
	std::mutex m_eventMutex;

	ID3D12Fence* m_fence;
	uint64_t m_nextFenceValue;
	uint64_t m_lastCompletedFenceValue;
	HANDLE m_fenceEventHandle;
};

class CommandListManager
{
};

#endif