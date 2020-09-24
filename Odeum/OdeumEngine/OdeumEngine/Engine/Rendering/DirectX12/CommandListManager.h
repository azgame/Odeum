#ifndef COMMANDLISTMANAGER_H
#define COMMANDLISTMANAGER_H

#include "D3DIncludes.h"
#include <stdint.h>
#include <queue>

class CommandAllocatorPool
{
public:
	CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type_);
	~CommandAllocatorPool();

	void Initialize(ID3D12Device* device_);
	void Uninitialize();

	ID3D12CommandAllocator* RequestAllocator(uint64_t completedFenceValue_);
	void DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_);

	inline size_t Size() { return m_pool.size(); }

private:

	const D3D12_COMMAND_LIST_TYPE m_type;

	ID3D12Device* m_device;
	std::vector<ID3D12CommandAllocator*> m_pool;
	std::queue<std::pair<uint64_t, ID3D12CommandAllocator*>> m_readyAllocators;
	std::mutex m_allocatorMutex;
};

class CommandQueue
{
	friend class CommandListManager;
	friend class CommandContext;

public:

	CommandQueue(D3D12_COMMAND_LIST_TYPE type_);
	~CommandQueue();

	void Initialize(ID3D12Device* device_);
	void Uninitialize();

	inline bool isReady() { return m_commandQueue != nullptr; }

	uint64_t IncrementFence();
	bool IsFenceComplete(uint64_t fenceValue_);
	void StallForFence(uint64_t fenceValue_);
	void StallForQueue(CommandQueue& queue_);
	void WaitForFence(uint64_t fenceValue_);
	void WaitForIdle() { WaitForFence(IncrementFence()); }

	ID3D12CommandQueue* GetCommandQueue() { return m_commandQueue; }

private:

	uint64_t ExecuteCommandList(ID3D12CommandList* cmdList_);
	ID3D12CommandAllocator* RequestAllocator();
	void DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_);

	ID3D12CommandQueue* m_commandQueue;
	const D3D12_COMMAND_LIST_TYPE m_type;
	CommandAllocatorPool m_allocatorPool;
	std::mutex m_fenceMutex;
	std::mutex m_eventMutex;

	ID3D12Fence* m_fence;	// fences are used for synchronization between the cpu and one or more gpus using a fence value (read/write from cpu and gpu).
							// Fence and fence event handle will frequently be used to delay cpu to wait for completion of commands on the gpu
	uint64_t m_nextFenceValue;
	uint64_t m_lastCompletedFenceValue;
	HANDLE m_fenceEventHandle;
};

class CommandListManager
{
	friend class GraphicsContext;

public:

	CommandListManager();
	~CommandListManager();

	void Initialize(ID3D12Device* device_);
	void Uninitialize();

	CommandQueue& GetGraphicsQueue() { return m_graphicsQueue; }
	CommandQueue& GetComputeQueue() { return m_computeQueue; }
	CommandQueue& GetCopyQueue() { return m_copyQueue; }

	CommandQueue& GetQueue(D3D12_COMMAND_LIST_TYPE type_ = D3D12_COMMAND_LIST_TYPE_DIRECT)
	{
		switch (type_)
		{
			case D3D12_COMMAND_LIST_TYPE_COMPUTE: return m_computeQueue;
			case D3D12_COMMAND_LIST_TYPE_COPY: return m_copyQueue;
			default: return m_graphicsQueue;
		}
	}

	ID3D12CommandQueue* GetCommandQueue()
	{
		return m_graphicsQueue.GetCommandQueue();
	}

	void CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type_, ID3D12GraphicsCommandList** cmdList_, ID3D12CommandAllocator** allocator_);

	bool IsFenceComplete(uint64_t fenceValue_)
	{
		return GetQueue(D3D12_COMMAND_LIST_TYPE(fenceValue_ >> 56)).IsFenceComplete(fenceValue_);
	}

	void WaitForFence(uint64_t fenceValue_);

	// The CPU will wait for all command queues to empty (so that the GPU is idle)
	void IdleGPU()
	{
		m_graphicsQueue.WaitForIdle();
		m_computeQueue.WaitForIdle();
		m_copyQueue.WaitForIdle();
	}

private:

	ID3D12Device* m_device;

	CommandQueue m_graphicsQueue;
	CommandQueue m_computeQueue;
	CommandQueue m_copyQueue;
};

#endif