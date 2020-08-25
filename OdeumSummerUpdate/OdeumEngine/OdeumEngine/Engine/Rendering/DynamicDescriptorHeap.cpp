#include "DynamicDescriptorHeap.h"

// Comments by Aidan Zizys, 08-20-2020

using namespace DXGraphics;

std::mutex DynamicDescriptorHeap::sm_mutex;

// static for safe thread use and access
std::vector<ID3D12DescriptorHeap*> DynamicDescriptorHeap::sm_descriptorHeapPool[2];
std::queue<std::pair<uint64_t, ID3D12DescriptorHeap*>> DynamicDescriptorHeap::sm_retiredDescriptorHeaps[2];
std::queue<ID3D12DescriptorHeap*> DynamicDescriptorHeap::sm_availableDescriptorHeaps[2];

DynamicDescriptorHeap::DynamicDescriptorHeap(GraphicsContext& context_, D3D12_DESCRIPTOR_HEAP_TYPE type_)
	: m_context(context_), m_descriptorType(type_)
{
	m_currentHeapPtr = nullptr;
	m_currentOffset = 0;
	m_descriptorSize = m_device->GetDescriptorHandleIncrementSize(type_);
}

DynamicDescriptorHeap::~DynamicDescriptorHeap()
{
}

void DynamicDescriptorHeap::CleanupUsedHeaps(uint64_t fenceValue_)
{
	RetireCurrentHeap();
	RetireUsedHeaps(fenceValue_);
	m_graphicsHandleCache.ClearCache();
}

// Bypass setting descriptor handles via tables and place a single handle directly into the heap. Not thread safe
D3D12_GPU_DESCRIPTOR_HANDLE DynamicDescriptorHeap::UploadDirect(D3D12_CPU_DESCRIPTOR_HANDLE handle_)
{
	if (!HasSpace(1))
	{
		RetireCurrentHeap();
		UnbindAllValid();
	}

	// This can trigger the creation of a new heap
	m_context.SetDescriptorHeap(m_descriptorType, GetHeapPointer()); // TODO Aidan: Add comment

	DescriptorHandle DestHandle = m_descriptorForHeapStart + m_currentOffset * m_descriptorSize;
	m_currentOffset += 1;

	m_device->CopyDescriptorsSimple(1, DestHandle.GetCpuHandle(), handle_, m_descriptorType);

	return DestHandle.GetGpuHandle();
}

// Called when we want a pointer to a heap
ID3D12DescriptorHeap* DynamicDescriptorHeap::RequestDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_)
{
	std::lock_guard<std::mutex> LockGuard(sm_mutex); // thread exclusivity beyond this point

	uint32_t heapType = type_ == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ? 1 : 0; // what type of heap do we want

	// If retired heaps have been cleaned up and fence signals they are ready for use, return them to the list of available heaps
	while (!sm_retiredDescriptorHeaps[heapType].empty() && g_CommandManager.IsFenceComplete(sm_retiredDescriptorHeaps[heapType].front().first))
	{
		sm_availableDescriptorHeaps[heapType].push(sm_retiredDescriptorHeaps[heapType].front().second);
		sm_retiredDescriptorHeaps[heapType].pop();
	}

	// Give me the first available heap if there is one
	if (!sm_availableDescriptorHeaps[heapType].empty())
	{
		ID3D12DescriptorHeap* heapPtr = sm_availableDescriptorHeaps[heapType].front();
		sm_availableDescriptorHeaps[heapType].pop();
		return heapPtr;
	}
	else // otherwise, create a new descriptor heap, emplace it into the pool, and return its address
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = type_;
		heapDesc.NumDescriptors = numDescriptorsPerHeap;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 1;
		ID3D12DescriptorHeap* heapPtr;
		if (FAILED(m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heapPtr))))
			Debug::Error("Could not create new descriptor heap", __FILENAME__, __LINE__);

		sm_descriptorHeapPool[heapType].emplace_back(heapPtr);
		return heapPtr;
	}
}

void DynamicDescriptorHeap::DiscardDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE type_, uint64_t fenceValue_, const std::vector<ID3D12DescriptorHeap*>& usedHeaps_)
{
	uint32_t heapType_ = type_ == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ? 1 : 0;

	std::lock_guard<std::mutex> LockGuard(sm_mutex); // thread exclusivity beyond this point

	for (auto usedHeap : usedHeaps_) // retire given heaps
		sm_retiredDescriptorHeaps[heapType_].push(std::make_pair(fenceValue_, usedHeap));
}

// Push the current heap into retirement
void DynamicDescriptorHeap::RetireCurrentHeap()
{
	if (m_currentOffset == 0)
	{
		assert(m_currentHeapPtr == nullptr);
		return;
	}

	assert(m_currentHeapPtr != nullptr);
	m_retiredHeaps.push_back(m_currentHeapPtr);
	m_currentHeapPtr = 0;
	m_currentOffset = 0;
}

// Discard all retired heaps
void DynamicDescriptorHeap::RetireUsedHeaps(uint64_t fenceValue_)
{
	DiscardDescriptorHeaps(m_descriptorType, fenceValue_, m_retiredHeaps);
	m_retiredHeaps.clear();
}

ID3D12DescriptorHeap* DynamicDescriptorHeap::GetHeapPointer()
{
	if (m_currentHeapPtr == nullptr) // if there is no current heap, trigger a request
	{
		assert(m_currentOffset == 0);
		m_currentHeapPtr = RequestDescriptorHeap(m_descriptorType);
		m_descriptorForHeapStart = DescriptorHandle(
			m_currentHeapPtr->GetCPUDescriptorHandleForHeapStart(),
			m_currentHeapPtr->GetGPUDescriptorHandleForHeapStart());
	}

	return m_currentHeapPtr;
}

void DynamicDescriptorHeap::CopyAndBindStagedTables(DescriptorHandleCache& handleCache_, ID3D12GraphicsCommandList* cmdList_, void(__stdcall ID3D12GraphicsCommandList::* setFunc_)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE))
{
	uint32_t neededSize = handleCache_.ComputeStagedSize();
	if (!HasSpace(neededSize)) // if there is no space in the current heap, retire it and unbind all valid handles
	{
		RetireCurrentHeap();
		UnbindAllValid();
		neededSize = handleCache_.ComputeStagedSize();
	}

	// This can trigger the creation of a new heap
	m_context.SetDescriptorHeap(m_descriptorType, GetHeapPointer());
	handleCache_.CopyAndBindStaleTables(m_descriptorType, m_descriptorSize, Allocate(neededSize), cmdList_, setFunc_);
}

void DynamicDescriptorHeap::UnbindAllValid()
{
	m_graphicsHandleCache.UnbindAllValid();
}

// Determine needed space for handle cache
uint32_t DynamicDescriptorHeap::DescriptorHandleCache::ComputeStagedSize()
{
	uint32_t neededSpace = 0;
	uint32_t rootIndex;
	uint32_t staleParams = m_staleRootParamsBitMap;

	// foreach stale root descriptor table, find number of handles in table and add count to neededspace. This determines how many handles we need in total
	while (_BitScanForward((unsigned long*)&rootIndex, staleParams)) // scan stale params from lsb to msb, assigning location of root param to rootIndex
	{
		staleParams ^= (1 << rootIndex);

		uint32_t maxSetHandle;

		// scan assigned handles bit map for given root desc table from msb to lsb, finding the index of the max set handle
		assert(_BitScanReverse((unsigned long*)&maxSetHandle, m_rootDescriptorTable[rootIndex].assignedHandlesBitMap)); 

		// add to needed space
		neededSpace += maxSetHandle + 1;
	}
	return neededSpace;
}

void DynamicDescriptorHeap::DescriptorHandleCache::CopyAndBindStaleTables(D3D12_DESCRIPTOR_HEAP_TYPE type_, uint32_t descSize_, DescriptorHandle destHandleStart_, ID3D12GraphicsCommandList* cmdList_, void(__stdcall ID3D12GraphicsCommandList::* setFunc_)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE))
{
	uint32_t staleParamCount = 0;
	uint32_t tableSize[DescriptorHandleCache::maxNumDescriptorTables];
	uint32_t rootIndices[DescriptorHandleCache::maxNumDescriptorTables];
	uint32_t neededSpace = 0;
	uint32_t rootIndex;

	uint32_t staleParams = m_staleRootParamsBitMap;

	// foreach stale root descriptor table, find number of handles in table and add count to neededspace. This determines how many handles we need in total
	// Similar to computing needed staged size
	while (_BitScanForward((unsigned long*)&rootIndex, staleParams))
	{
		rootIndices[staleParamCount] = rootIndex;
		staleParams ^= (1 << rootIndex);

		uint32_t MaxSetHandle;
		assert(TRUE == _BitScanReverse((unsigned long*)&MaxSetHandle, m_rootDescriptorTable[rootIndex].assignedHandlesBitMap),
			"Root entry marked as stale but has no stale descriptors");

		neededSpace += MaxSetHandle + 1;
		tableSize[staleParamCount] = MaxSetHandle + 1; // set the table size needed foreach root descriptor table

		++staleParamCount;
	}

	assert(staleParamCount <= DescriptorHandleCache::maxNumDescriptorTables,
		"Only so many descriptor tables allowed");

	m_staleRootParamsBitMap = 0;

	static const uint32_t kMaxDescriptorsPerCopy = 16;

	UINT NumDestDescriptorRanges = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE pDestDescriptorRangeStarts[kMaxDescriptorsPerCopy];
	UINT pDestDescriptorRangeSizes[kMaxDescriptorsPerCopy];

	UINT NumSrcDescriptorRanges = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE pSrcDescriptorRangeStarts[kMaxDescriptorsPerCopy];
	UINT pSrcDescriptorRangeSizes[kMaxDescriptorsPerCopy];

	for (uint32_t i = 0; i < staleParamCount; ++i)
	{
		rootIndex = rootIndices[i];												// the root index of each stale param
		(cmdList_->*setFunc_)(rootIndex, destHandleStart_.GetGpuHandle());		// Use the given set function for the given command list on the current gpu visible handle

		DescriptorTableCache& RootDescTable = m_rootDescriptorTable[rootIndex];	// determine the root descriptor table to work on

		D3D12_CPU_DESCRIPTOR_HANDLE* SrcHandles = RootDescTable.tableStart;		// The starting location of the handles for the currently worked on root descriptor table
		uint64_t SetHandles = (uint64_t)RootDescTable.assignedHandlesBitMap;	// Which handles are set - assigned handles bit map
		D3D12_CPU_DESCRIPTOR_HANDLE CurDest = destHandleStart_.GetCpuHandle();	// current destination handle
		destHandleStart_ += tableSize[i] * descSize_;							// increment dest handle (for next iteration)

		unsigned long SkipCount;
		while (_BitScanForward64(&SkipCount, SetHandles))
		{
			// Skip over unset descriptor handles
			SetHandles >>= SkipCount;						// bitshift set handles to the currently set handle so that next bitscan finds next set handle
			SrcHandles += SkipCount;						// Increment source handles past handles to skip
			CurDest.ptr += SkipCount * descSize_;			// increment destination address past handles to skip

			unsigned long DescriptorCount;
			_BitScanForward64(&DescriptorCount, ~SetHandles);	// Once we've reached a set handle, determine how many set handles there are in a row by reaching the next empty handle
			SetHandles >>= DescriptorCount;						// Shift past them for next iteration

			// If we run out of temp room, copy what we've got so far
			if (NumSrcDescriptorRanges + DescriptorCount > kMaxDescriptorsPerCopy)
			{
				m_device->CopyDescriptors(
					NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
					NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes,
					type_);

				NumSrcDescriptorRanges = 0;
				NumDestDescriptorRanges = 0;
			}

			// Setup destination range
			pDestDescriptorRangeStarts[NumDestDescriptorRanges] = CurDest; // index incrementing by 1, location of start is curdest
			pDestDescriptorRangeSizes[NumDestDescriptorRanges] = DescriptorCount; // size is descriptor count (how many in a row, i.e. range)
			++NumDestDescriptorRanges;

			// Setup source ranges (one descriptor each because we don't assume they are contiguous)
			for (uint32_t j = 0; j < DescriptorCount; ++j)
			{
				pSrcDescriptorRangeStarts[NumSrcDescriptorRanges] = SrcHandles[j];
				pSrcDescriptorRangeSizes[NumSrcDescriptorRanges] = 1;
				++NumSrcDescriptorRanges;
			}

			// Move the destination pointer forward by the number of descriptors we will copy
			SrcHandles += DescriptorCount;
			CurDest.ptr += DescriptorCount * descSize_;
		}
	}

	m_device->CopyDescriptors(
		NumDestDescriptorRanges, pDestDescriptorRangeStarts, pDestDescriptorRangeSizes,
		NumSrcDescriptorRanges, pSrcDescriptorRangeStarts, pSrcDescriptorRangeSizes,
		type_);
}

// Mark all root descriptor tables with assignments as stale
void DynamicDescriptorHeap::DescriptorHandleCache::UnbindAllValid()
{
	m_staleRootParamsBitMap = 0;
	unsigned long tableParams = m_rootDescriptorTablesBitMap;
	unsigned long rootIndex;

	while (_BitScanForward(&rootIndex, tableParams))
	{
		tableParams ^= (1 << rootIndex);
		if (m_rootDescriptorTable[rootIndex].assignedHandlesBitMap != 0)
			m_staleRootParamsBitMap |= (1 << rootIndex);
	}
}

void DynamicDescriptorHeap::DescriptorHandleCache::StageDescriptorHandles(UINT rootIndex_, UINT offset_, UINT numHandles_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
{
	assert(((1 << rootIndex_) & m_rootDescriptorTablesBitMap) != 0, "Root parameter is not a CBV_SRV_UAV descriptor table");
	assert(offset_ + numHandles_ <= m_rootDescriptorTable[rootIndex_].tableSize);

	// Copy given handles into table cache at root index
	DescriptorTableCache& TableCache = m_rootDescriptorTable[rootIndex_];
	D3D12_CPU_DESCRIPTOR_HANDLE* CopyDest = TableCache.tableStart + offset_;
	for (UINT i = 0; i < numHandles_; ++i)
		CopyDest[i] = handles_[i];

	// Mark assignments: mark number of handles at offset
	TableCache.assignedHandlesBitMap |= ((1 << numHandles_) - 1) << offset_;
	// Mark table as stale
	m_staleRootParamsBitMap |= (1 << rootIndex_);
}

void DynamicDescriptorHeap::DescriptorHandleCache::ParseRootSignature(D3D12_DESCRIPTOR_HEAP_TYPE type_, const RootSignature& rootSig_)
{
	UINT CurrentOffset = 0;

	assert(rootSig_.m_numParameters <= 16);

	m_staleRootParamsBitMap = 0;
	m_rootDescriptorTablesBitMap = (type_ == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ?
		rootSig_.m_SamplerTableBitMap : rootSig_.m_descriptorTableBitMap);

	unsigned long tableParams = m_rootDescriptorTablesBitMap;
	unsigned long rootIndex;
	while (_BitScanForward(&rootIndex, tableParams))
	{
		tableParams ^= (1 << rootIndex);

		UINT TableSize = rootSig_.m_descriptorTableSize[rootIndex];
		assert(TableSize > 0);

		// Get a table cache at used root index and set the table start/size based on the tables in the root sig
		DescriptorTableCache& rootDescriptorTable = m_rootDescriptorTable[rootIndex]; 
		rootDescriptorTable.assignedHandlesBitMap = 0;
		rootDescriptorTable.tableStart = m_handleCache + CurrentOffset;
		rootDescriptorTable.tableSize = TableSize;

		CurrentOffset += TableSize;
	}

	m_maxCachedDescriptors = CurrentOffset;

	assert(m_maxCachedDescriptors <= maxNumDescriptors);
}
