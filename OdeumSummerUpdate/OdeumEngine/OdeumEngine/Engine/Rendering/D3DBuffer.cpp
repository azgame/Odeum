#include "pch.h"
#include "D3DBuffer.h"

#include "D3DRenderAPI.h"
#include "GraphicsContext.h"

void D3DBuffer::Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_)
{
	Destroy(); // Clean

	m_eCount = numElements_;
	m_eSize = elementSize_;
	m_bufferSize = numElements_ * elementSize_;

	D3D12_RESOURCE_DESC rDesc = CreateBufferDescription();

	m_usageState = D3D12_RESOURCE_STATE_COMMON;

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	ID3D12Device5* device = DXGraphics::m_device;

	if (FAILED(
		device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &rDesc, m_usageState, nullptr, IID_PPV_ARGS(&m_pResource)))
	)	
		Debug::Error("Could not create buffer", __FILENAME__, __LINE__);

	m_vGpuAddress = m_pResource->GetGPUVirtualAddress();

	device = nullptr;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DBuffer::CreateCBV(uint32_t offset_, uint32_t size_) const
{
	if (offset_ + size_ > m_bufferSize)
		Debug::Error("Attempted allocation of descriptor failed!", __FILENAME__, __LINE__);

	size_ = ALIGN(size_, 16);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = offset_;
	cbvDesc.SizeInBytes = size_;

	ID3D12Device5* device = DXGraphics::m_device;

	D3D12_CPU_DESCRIPTOR_HANDLE hcbv = DXGraphics::Allocate(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	device->CreateConstantBufferView(&cbvDesc, hcbv);

	device = nullptr;

	return hcbv;
}

D3D12_VERTEX_BUFFER_VIEW D3DBuffer::VertexBufferView(size_t offset_, uint32_t size_, uint32_t stride_) const
{
	D3D12_VERTEX_BUFFER_VIEW vertBufferView;
	vertBufferView.BufferLocation = offset_;
	vertBufferView.SizeInBytes = size_;
	vertBufferView.StrideInBytes = stride_;
	return vertBufferView;
}

D3D12_INDEX_BUFFER_VIEW D3DBuffer::IndexBufferView(size_t offset_, uint32_t size_, bool b32Bit) const
{
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = offset_;
	indexBufferView.SizeInBytes = size_;
	indexBufferView.Format = b32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
	return indexBufferView;
}

D3D12_RESOURCE_DESC D3DBuffer::CreateBufferDescription()
{
	D3D12_RESOURCE_DESC Desc = {};
	Desc.Alignment = 0;
	Desc.DepthOrArraySize = 1;
	Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	Desc.Flags = m_resourceFlags;
	Desc.Format = DXGI_FORMAT_UNKNOWN;
	Desc.Height = 1;
	Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Width = (UINT64)m_bufferSize;
	return Desc;
}

void ByteAddressedBuffer::CreateDerivedView()
{
}

void StructuredBuffer::CreateDerivedView()
{
}
