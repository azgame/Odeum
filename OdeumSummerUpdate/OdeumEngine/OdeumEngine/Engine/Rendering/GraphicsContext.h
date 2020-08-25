#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "../../pch.h"

class ContextManager
{

};

class GraphicsContext
{
public:

	static GraphicsContext* Initialize(std::string name_ = "");

	void SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type_, ID3D12DescriptorHeap* heap_);

	GraphicsContext() {}
	~GraphicsContext();

private:
};

#endif