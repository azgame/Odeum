#include "CommandAllocatorManager.h"

CommandAllocatorManager::CommandAllocatorManager(D3D12_COMMAND_LIST_TYPE type_) : m_type(type_), m_device(nullptr)
{
}

CommandAllocatorManager::~CommandAllocatorManager()
{
	Uninitialize();
}

void CommandAllocatorManager::Initialize(ID3D12Device* device_)
{
	m_device = device_;
}

void CommandAllocatorManager::Uninitialize()
{
	for (auto allocator : m_pool)
		allocator->Release();

	m_pool.clear();
}

ID3D12CommandAllocator* CommandAllocatorManager::RequestAllocator(uint64_t completedFenceValue_)
{
	std::lock_guard<std::mutex> LockGuard(m_allocatorMutex);

	ID3D12CommandAllocator* allocator = nullptr;

	if (!m_readyAllocators.empty())
	{
		std::pair<uint64_t, ID3D12CommandAllocator*>& allocatorPair = m_readyAllocators.front();

		if (allocatorPair.first <= completedFenceValue_)
		{
			allocator = allocatorPair.second;
			if (FAILED(allocator->Reset()))
				Debug::Error("Command allocator could not be reset!", __FILENAME__, __LINE__);

			m_readyAllocators.pop();
		}
	}

	if (allocator == nullptr)
	{
		if (FAILED(m_device->CreateCommandAllocator(m_type, IID_PPV_ARGS(&allocator))))
			Debug::Error("Command Allocator could not be created!", __FILENAME__, __LINE__);

		wchar_t allocatorName[32];
		swprintf(allocatorName, 32, L"CommandAllocator %zu", m_pool.size());
		allocator->SetName(allocatorName);
		m_pool.push_back(allocator);
	}

	return allocator;
}

void CommandAllocatorManager::DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_)
{
	std::lock_guard<std::mutex> LockGuard(m_allocatorMutex);

	m_readyAllocators.push(std::make_pair(fenceValue_, allocator_));
}
