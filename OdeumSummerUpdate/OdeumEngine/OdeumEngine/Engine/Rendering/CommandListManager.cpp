#include "CommandListManager.h"

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

uint64_t CommandQueue::ExecuteCommandList(ID3D12CommandList* cmdList_)
{
	return uint64_t();
}

ID3D12CommandAllocator* CommandQueue::RequestAllocator()
{
	return nullptr;
}

void CommandQueue::DiscardAllocator(uint64_t fenceValue_, ID3D12CommandAllocator* allocator_)
{
}
