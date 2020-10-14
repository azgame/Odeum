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

#include "CommandListManager.h"

#include <algorithm>

#include "D3DCore.h"



namespace DXGraphics
{
	extern CommandListManager m_commandManager;
}

CommandAllocatorPool::CommandAllocatorPool(D3D12_COMMAND_LIST_TYPE type_) : m_type(type_), m_device(nullptr)
{
}

CommandAllocatorPool::~CommandAllocatorPool()
{
	Uninitialize();
}

void CommandAllocatorPool::Initialize(ID3D12Device* device_)
{
	m_device = device_;
}

void CommandAllocatorPool::Uninitialize()
{
	for (auto allocator : m_pool)
		allocator->Release();

	m_pool.clear();
}

ID3D12CommandAllocator* CommandAllocatorPool::RequestAllocator(uint64_t completedFenceValue_)
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

void CommandAllocatorPool::DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_)
{
	std::lock_guard<std::mutex> LockGuard(m_allocatorMutex);

	m_readyAllocators.push(std::make_pair(fenceValue_, allocator_));
}


CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type_) :
	m_type(type_),
	m_commandQueue(nullptr),
	m_fence(nullptr),
	m_nextFenceValue((uint64_t)type_ << 56 | 1), // set nextfencevalue to a large value based on commandlist type, +1 (eg. 1000...0001)
	m_lastCompletedFenceValue((uint64_t)type_ << 56), // set last completed to large value based on commandlist type, no +1 (eg. 1000...0000)
	m_allocatorPool(type_)
{
}

CommandQueue::~CommandQueue()
{
	Uninitialize();
}

void CommandQueue::Initialize(ID3D12Device* device_)
{
	ASSERT(device_ != nullptr, "Device is null!");
	ASSERT(!isReady(), "Command queue as been initialized!");
	ASSERT(m_allocatorPool.Size() == 0, "Allocator pool size if not 0!");

	// Create our command queue
	D3D12_COMMAND_QUEUE_DESC qDesc = {};
	qDesc.Type = m_type;
	qDesc.NodeMask = 1;
	device_->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_commandQueue));
	m_commandQueue->SetName(L"Command Queue");

	// create our fence
	if (FAILED(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
		Debug::Error("Fence could not be created", __FILENAME__, __LINE__);

	m_fence->SetName(L"CommandList Fence");
	m_fence->Signal((uint64_t)m_type << 56);

	// Initialize the fence event handle
	m_fenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
	ASSERT(m_fenceEventHandle != NULL, "Fence event handle is null!");

	// Initialize our allocator pool
	m_allocatorPool.Initialize(device_);

	ASSERT(isReady(), "Command queue failed to initialize!");
}

void CommandQueue::Uninitialize()
{
	if (m_commandQueue == nullptr)
		return;

	m_allocatorPool.Uninitialize();

	CloseHandle(m_fenceEventHandle);

	m_fence->Release();
	m_fence = nullptr;

	m_commandQueue->Release();
	m_commandQueue = nullptr;
}

// Increment fence value, thread safe
uint64_t CommandQueue::IncrementFence()
{
	std::lock_guard<std::mutex> LockGuard(m_fenceMutex);
	m_commandQueue->Signal(m_fence, m_nextFenceValue);
	return m_nextFenceValue++;
}

// check if the fence is complete
bool CommandQueue::IsFenceComplete(uint64_t fenceValue_)
{
	if (fenceValue_ > m_lastCompletedFenceValue)
		m_lastCompletedFenceValue = max(m_lastCompletedFenceValue, m_fence->GetCompletedValue());

	return fenceValue_ <= m_lastCompletedFenceValue;
}

// Tell a queue to wait for a fence value
void CommandQueue::StallForFence(uint64_t fenceValue_)
{
	CommandQueue& queue = DXGraphics::m_commandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue_ >> 56));
	m_commandQueue->Wait(queue.m_fence, fenceValue_);
}

// Tell a queue to wait on another queue
void CommandQueue::StallForQueue(CommandQueue& queue_)
{
	ASSERT(queue_.m_nextFenceValue > 0, "Next fence value is 0 or less!");
	m_commandQueue->Wait(queue_.m_fence, queue_.m_nextFenceValue - 1);
}

// Wait for fence completion
void CommandQueue::WaitForFence(uint64_t fenceValue_)
{
	if (IsFenceComplete(fenceValue_))
		return;

	std::lock_guard<std::mutex> LockGuard(m_eventMutex);

	m_fence->SetEventOnCompletion(fenceValue_, m_fenceEventHandle);
	WaitForSingleObject(m_fenceEventHandle, INFINITE);
	m_lastCompletedFenceValue = fenceValue_;
}

// Execute the given command list and signal the queue fence values
uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* cmdList_)
{
	std::lock_guard<std::mutex> LockGuard(m_fenceMutex);

	if (FAILED(((ID3D12GraphicsCommandList*)cmdList_)->Close()))
		Debug::Error("Command List could not be closed for execution", __FILENAME__, __LINE__);

	m_commandQueue->ExecuteCommandLists(1, &cmdList_);
	m_commandQueue->Signal(m_fence, m_nextFenceValue);

	return m_nextFenceValue++;
}

ID3D12CommandAllocator* CommandQueue::RequestAllocator()
{
	uint64_t completedFence = m_fence->GetCompletedValue();
	return m_allocatorPool.RequestAllocator(completedFence);
}

void CommandQueue::DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_)
{
	m_allocatorPool.DiscardAllocator(fenceValue_, allocator_);
}

CommandListManager::CommandListManager() :
	m_device(nullptr),
	m_graphicsQueue(D3D12_COMMAND_LIST_TYPE_DIRECT),
	m_computeQueue(D3D12_COMMAND_LIST_TYPE_COMPUTE),
	m_copyQueue(D3D12_COMMAND_LIST_TYPE_COPY)
{
}

CommandListManager::~CommandListManager()
{
	Uninitialize();
}

void CommandListManager::Initialize(ID3D12Device* device_)
{
	ASSERT(device_ != nullptr, "Device is null!");

	m_device = device_;

	m_graphicsQueue.Initialize(device_);
	m_computeQueue.Initialize(device_);
	m_copyQueue.Initialize(device_);
}

void CommandListManager::Uninitialize()
{
	m_graphicsQueue.Uninitialize();
	m_computeQueue.Uninitialize();
	m_copyQueue.Uninitialize();
}

// Create a new command list of the given type
void CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type_, ID3D12GraphicsCommandList** cmdList_, ID3D12CommandAllocator** allocator_)
{
	ASSERT(type_ != D3D12_COMMAND_LIST_TYPE_BUNDLE, "Bundles are not yet supported");

	switch (type_)
	{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: *allocator_ = m_graphicsQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: *allocator_ = m_computeQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_COPY: *allocator_ = m_copyQueue.RequestAllocator(); break;
	}

	if (FAILED(m_device->CreateCommandList(1, type_, *allocator_, nullptr, IID_PPV_ARGS(cmdList_))))
		Debug::Error("Failed to create new command list", __FILENAME__, __LINE__);

	(*cmdList_)->SetName(L"CommandList");
}

// Tell the specified queue to wait for the fence
void CommandListManager::WaitForFence(uint64_t fenceValue_)
{
	CommandQueue& queue = DXGraphics::m_commandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue_ >> 56));
	queue.WaitForFence(fenceValue_);
}
