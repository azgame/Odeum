#ifndef READBACKBUFFER_H
#define READBACKBUFFER_H

#include "D3DBuffer.h"

class ReadbackBuffer : public D3DBuffer
{
public:
	virtual ~ReadbackBuffer() { ResetResource(); }

	void Create(uint32_t numElements_, uint32_t elementSize_);

	void* Map()
	{
		void* mem;
		m_resource->Map(0, &CD3DX12_RANGE(0, allocatedSize), &mem);
		return mem;
	}

	void UnMap()
	{
		m_resource->Unmap(0, &CD3DX12_RANGE(0, 0));
	}
};

#endif