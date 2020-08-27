#include "CommandListManager.h"

#include "D3DCore.h"

namespace DXGraphics
{
	extern CommandListManager m_commandManager;
}

CommandQueue::CommandQueue(D3D12_COMMAND_LIST_TYPE type_) :
	m_type(type_),
	m_commandQueue(nullptr),
	m_fence(nullptr),
	m_nextFenceValue((uint64_t)type_ << 56 | 1), // not sure why we do this
	m_lastCompletedFenceValue((uint64_t)type_ << 56), // not sure why we do this
	m_allocatorPool(type_)
{
}

CommandQueue::~CommandQueue()
{
	Uninitialize();
}

void CommandQueue::Initialize(ID3D12Device* device_)
{
	assert(device_ != nullptr);
	assert(!isReady());
	assert(m_allocatorPool.Size() == 0);

	D3D12_COMMAND_QUEUE_DESC qDesc = {};
	qDesc.Type = m_type;
	qDesc.NodeMask = 1;
	device_->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_commandQueue));
	m_commandQueue->SetName(L"Command Queue");

	if (FAILED(device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence))))
		Debug::Error("Fence could not be created", __FILENAME__, __LINE__);

	m_fence->SetName(L"CommandList Fence");
	m_fence->Signal((uint64_t)m_type << 56);

	m_fenceEventHandle = CreateEvent(nullptr, false, false, nullptr);
	assert(m_fenceEventHandle != NULL);

	m_allocatorPool.Initialize(device_);

	assert(isReady());
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

uint64_t CommandQueue::IncrementFence()
{
	std::lock_guard<std::mutex> LockGuard(m_fenceMutex);
	m_commandQueue->Signal(m_fence, m_nextFenceValue);
	return m_nextFenceValue++;
}

bool CommandQueue::IsFenceComplete(uint64_t fenceValue_)
{
	if (fenceValue_ > m_lastCompletedFenceValue)
		m_lastCompletedFenceValue = max(m_lastCompletedFenceValue, m_fence->GetCompletedValue());

	return fenceValue_ <= m_lastCompletedFenceValue;
}

void CommandQueue::StallForFence(uint64_t fenceValue_)
{
	CommandQueue& queue = DXGraphics::m_commandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue_ >> 56));
	m_commandQueue->Wait(queue.m_fence, fenceValue_);
}

void CommandQueue::StallForQueue(CommandQueue& queue_)
{
	assert(queue_.m_nextFenceValue > 0);
	m_commandQueue->Wait(queue_.m_fence, queue_.m_nextFenceValue - 1);
}

void CommandQueue::WaitForFence(uint64_t fenceValue_)
{
	if (IsFenceComplete(fenceValue_))
		return;

	std::lock_guard<std::mutex> LockGuard(m_eventMutex);

	m_fence->SetEventOnCompletion(fenceValue_, m_fenceEventHandle);
	WaitForSingleObject(m_fenceEventHandle, INFINITE);
	m_lastCompletedFenceValue = fenceValue_;
}

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
	assert(device_ != nullptr);

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

void CommandListManager::CreateNewCommandList(D3D12_COMMAND_LIST_TYPE type_, ID3D12GraphicsCommandList** cmdList_, ID3D12CommandAllocator** allocator_)
{
	assert(type_ != D3D12_COMMAND_LIST_TYPE_BUNDLE, "Bundles are not yet supported");

	switch (type_)
	{
		case D3D12_COMMAND_LIST_TYPE_DIRECT: *allocator_ = m_graphicsQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_BUNDLE: break;
		case D3D12_COMMAND_LIST_TYPE_COMPUTE: *allocator_ = m_computeQueue.RequestAllocator(); break;
		case D3D12_COMMAND_LIST_TYPE_COPY: *allocator_ = m_copyQueue.RequestAllocator(); break;
	}

	if (FAILED(m_device->CreateCommandList(1, type_, *allocator_, nullptr, IID_PPV_ARGS(&cmdList_))))
		Debug::Error("Failed to create new command list", __FILENAME__, __LINE__);

	(*cmdList_)->SetName(L"CommandList");
}

void CommandListManager::WaitForFence(uint64_t fenceValue_)
{
	CommandQueue& queue = DXGraphics::m_commandManager.GetQueue((D3D12_COMMAND_LIST_TYPE)(fenceValue_ >> 56));
	queue.WaitForFence(fenceValue_);
}
