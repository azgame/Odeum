#include "ReadbackBuffer.h"
#include "../D3DCore.h"

void ReadbackBuffer::Create(uint32_t numElements_, uint32_t elementSize_)
{
	ResetResource();

	elementCount = numElements_;
	elementByteSize = elementSize_;
	allocatedSize = elementCount * elementByteSize;
	currentState = D3D12_RESOURCE_STATE_COPY_DEST;

	D3D12_RESOURCE_DESC desc = ResourceDescription();
	desc.Flags = flags;

	D3D12_RESOURCE_DESC ResourceDesc = {};
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = allocatedSize;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	if (FAILED(DXGraphics::m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK), D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		currentState, nullptr, IID_PPV_ARGS(&m_resource))))
	{
		ERROR("Could not create read back buffer");
		throw std::runtime_error("Could not create read back buffer");
	}

	m_gpuAddress = m_resource->GetGPUVirtualAddress();

#ifdef _DEBUG
	m_resource->SetName(L"Read back buffer");
#endif
}