#include "pch.h"
#include "D3DBuffer.h"


void D3DBuffer::Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_)
{
	Destroy(); // Clean

	m_eCount = numElements_;
	m_eSize = elementSize_;
	m_bufferSize = numElements_ * elementSize_;

	D3D12_RESOURCE_DESC rDesc = CreateBufferDescription();

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	// Todo Aidan access device to create committed resource
	// if (FAILED())
	//	Debug
	// m_vgpuAddress = m_pResource->GetGpuVirtualAddress();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DBuffer::CreateCBV(uint32_t offset_, uint32_t size_) const
{
	if (offset_ + size_ > m_bufferSize)
		Debug::Error("Attempted allocation of descriptor failed!", __FILENAME__, __LINE__);

	size_ = ALIGN(size_, 16);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = offset_;
	cbvDesc.SizeInBytes = size_;

	// Todo Aidan implement a descriptor allocation in the core graphics library or in the context
	//D3D12_CPU_DESCRIPTOR_HANDLE hcbv = AllocateDescriptor(*data*);
	//device->CreateCBV();
	//return hcbv;

	return D3D12_CPU_DESCRIPTOR_HANDLE();
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
