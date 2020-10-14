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

#ifndef DESCRIPTORALLOCATOR_H
#define DESCRIPTORALLOCATOR_H

#include "D3DIncludes.h"

// Acts as a place for allocating cpu visible descriptors, not for gpu use. Allocated handles will later be bound to gpu visible heaps
// Has a pool of descriptors and can, in a thread safe way, allocate descriptors into that heap for use by other objects
class DescriptorAllocator
{
public:

	DescriptorAllocator(D3D12_DESCRIPTOR_HEAP_TYPE type_) : m_type(type_), m_currentHeap(nullptr) {}

	D3D12_CPU_DESCRIPTOR_HANDLE Allocate(UINT count_);

	static void DestroyHeaps();

private:

	static ID3D12DescriptorHeap* GetNewHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_);
	static const uint32_t kDescriptorsPerHeap = 256;
	static std::vector<ID3D12DescriptorHeap*> m_descriptorHeapPool;
	static std::mutex m_allocatorMutex;

	ID3D12DescriptorHeap* m_currentHeap;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentHandle;
	uint32_t m_descriptorSize;
	uint32_t m_numFreeHandles;
};

#endif