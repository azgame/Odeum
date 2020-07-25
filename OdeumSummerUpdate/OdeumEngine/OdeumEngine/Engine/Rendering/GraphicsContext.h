#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"

class ContextManager
{

};

class GraphicsContext
{
public:

	static GraphicsContext* Initialize();

	GraphicsContext() {}
	~GraphicsContext();

	D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type_, UINT count_ = 1);

private:
};

#endif