#ifndef COMMANDALLOCATORPOOL_H
#define COMMANDALLOCATORPOOL_H

#include "DXIncludes.h"
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

#endif