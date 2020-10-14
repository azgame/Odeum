#ifndef READBACKBUFFER_H
#define READBACKBUFFER_H

#include "D3DBuffer.h"

class ReadbackBuffer : public D3DBuffer
{
public:
	virtual ~ReadbackBuffer() { Destroy(); }

	void Create(uint32_t numElements_, uint32_t elementSize_);

	void* Map();
	void UnMap();
};

#endif