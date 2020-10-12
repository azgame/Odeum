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


#ifndef DYNAMICDESCRIPTORHEAP_H
#define DYNAMICDESCRIPTORHEAP_H

#include "D3DIncludes.h"

#include "DescriptorAllocator.h"
#include "RootSignature.h"

#include <queue>

class CommandContext;

// TODO Aidan: Add compute

class DynamicDescriptorHeap
{
public:
	DynamicDescriptorHeap(CommandContext& context_, D3D12_DESCRIPTOR_HEAP_TYPE type_);
	~DynamicDescriptorHeap();

	static void Destroy()
	{
		sm_descriptorHeapPool[0].clear();
		sm_descriptorHeapPool[1].clear();
	}

	void CleanupUsedHeaps(uint64_t fenceValue_);

	void SetGraphicsDescriptorHandle(UINT rootIndex_, UINT offset_, UINT numHandles_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[])
	{
		m_graphicsHandleCache.StageDescriptorHandles(rootIndex_, offset_, numHandles_, handles_);
	}

	D3D12_GPU_DESCRIPTOR_HANDLE UploadDirect(D3D12_CPU_DESCRIPTOR_HANDLE handle_);

	void CommitGraphicsDescriptorTables(ID3D12GraphicsCommandList* cmdList_)
	{
		if (m_graphicsHandleCache.m_staleRootParamsBitMap != 0)
			CopyAndBindStagedTables(m_graphicsHandleCache, cmdList_, &ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable);
	}

	void ParseGraphicsRootSignature(const RootSignature& rootSig_)
	{
		m_graphicsHandleCache.ParseRootSignature(m_descriptorType, rootSig_);
	}

private:

	static const uint32_t numDescriptorsPerHeap = 1024;
	static std::mutex sm_mutex;
	static std::vector<ID3D12DescriptorHeap*> sm_descriptorHeapPool[2];
	static std::queue<std::pair<uint64_t, ID3D12DescriptorHeap*>> sm_retiredDescriptorHeaps[2];
	static std::queue<ID3D12DescriptorHeap*> sm_availableDescriptorHeaps[2];

	// Static methods
	static ID3D12DescriptorHeap* RequestDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_);
	static void DiscardDescriptorHeaps(D3D12_DESCRIPTOR_HEAP_TYPE type_, uint64_t fenceValue_, const std::vector<ID3D12DescriptorHeap*>& usedHeaps_);

	CommandContext& m_context;
	ID3D12DescriptorHeap* m_currentHeapPtr;
	const D3D12_DESCRIPTOR_HEAP_TYPE m_descriptorType;
	uint32_t m_descriptorSize;
	uint32_t m_currentOffset;
	DescriptorHandle m_descriptorForHeapStart;
	std::vector<ID3D12DescriptorHeap*> m_retiredHeaps;

	// Cache used to track table location and size (using indirection) and bitmap for knowing which handles are assigned
	struct DescriptorTableCache
	{
		DescriptorTableCache() : assignedHandlesBitMap(0) {}
		uint32_t assignedHandlesBitMap;							// in the space we take up, how many of our table params are assigned?
		D3D12_CPU_DESCRIPTOR_HANDLE* tableStart;				// where we exist
		uint32_t tableSize;										// how big are we?
	};

	// Cache used to track descriptor table caches and their assignment
	// Used to parse an incoming root signature to match the handle layout of the heap and interior descriptor tables
	struct DescriptorHandleCache
	{
		DescriptorHandleCache()
		{
			ClearCache();
		}

		void ClearCache()
		{
			m_rootDescriptorTablesBitMap = 0;
			m_maxCachedDescriptors = 0;
		}

		uint32_t m_rootDescriptorTablesBitMap;
		uint32_t m_staleRootParamsBitMap;
		uint32_t m_maxCachedDescriptors;

		static const uint32_t maxNumDescriptors = 256;
		static const uint32_t maxNumDescriptorTables = 16;

		uint32_t ComputeStagedSize();
		void CopyAndBindStaleTables(D3D12_DESCRIPTOR_HEAP_TYPE type_, uint32_t descSize_, DescriptorHandle destHandleStart_, ID3D12GraphicsCommandList* cmdList_,
			void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* setFunc_)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

		DescriptorTableCache m_rootDescriptorTable[maxNumDescriptorTables];
		D3D12_CPU_DESCRIPTOR_HANDLE m_handleCache[maxNumDescriptors];

		void UnbindAllValid();
		void StageDescriptorHandles(UINT rootIndex_, UINT offset_, UINT numHandles_, const D3D12_CPU_DESCRIPTOR_HANDLE handles_[]);
		void ParseRootSignature(D3D12_DESCRIPTOR_HEAP_TYPE type_, const RootSignature& rootSig_);
	};

	DescriptorHandleCache m_graphicsHandleCache;

	bool HasSpace(uint32_t count_)
	{
		return (m_currentHeapPtr != nullptr && m_currentOffset + count_ <= numDescriptorsPerHeap);
	}

	void RetireCurrentHeap();
	void RetireUsedHeaps(uint64_t fenceValue_);
	ID3D12DescriptorHeap* GetHeapPointer();

	DescriptorHandle Allocate(UINT count_)
	{
		DescriptorHandle ret = m_descriptorForHeapStart + m_currentOffset * m_descriptorSize;
		m_currentOffset += count_;
		return ret;
	}

	void CopyAndBindStagedTables(DescriptorHandleCache& handleCache_, ID3D12GraphicsCommandList* cmdList_,
		void (STDMETHODCALLTYPE ID3D12GraphicsCommandList::* setFunc_)(UINT, D3D12_GPU_DESCRIPTOR_HANDLE));

	void UnbindAllValid();
};

#endif