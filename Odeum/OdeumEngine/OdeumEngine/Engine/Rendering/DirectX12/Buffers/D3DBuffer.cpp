#include "D3DBuffer.h"

#include "../D3DCore.h"
#include "../DXCommandContext.h"

using namespace DXGraphics;

// In basic dx terms, this is all very straightforward

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

	if (FAILED(
		DXGraphics::m_device->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &rDesc, m_usageState, nullptr, IID_PPV_ARGS(&m_resource)))
	)	
		Debug::Error("Could not create buffer", __FILENAME__, __LINE__);

	m_vGpuAddress = m_resource->GetGPUVirtualAddress();

	if (initialData_)
		CommandContext::InitializeBuffer(*this, initialData_, m_bufferSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DBuffer::CreateCBV(uint32_t offset_, uint32_t size_) const
{
	if (offset_ + size_ > m_bufferSize)
		Debug::Error("Attempted allocation of descriptor failed!", __FILENAME__, __LINE__);

	size_ = Utility::Align(size_, 16);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = offset_;
	cbvDesc.SizeInBytes = size_;

	D3D12_CPU_DESCRIPTOR_HANDLE hcbv = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateConstantBufferView(&cbvDesc, hcbv);

	return hcbv;
}

D3D12_VERTEX_BUFFER_VIEW D3DBuffer::VertexBufferView(size_t offset_, uint32_t size_, uint32_t stride_) const
{
	D3D12_VERTEX_BUFFER_VIEW vertBufferView;
	vertBufferView.BufferLocation = m_vGpuAddress + offset_;
	vertBufferView.SizeInBytes = size_;
	vertBufferView.StrideInBytes = stride_;
	return vertBufferView;
}

D3D12_INDEX_BUFFER_VIEW D3DBuffer::IndexBufferView(size_t offset_, uint32_t size_, bool b32Bit) const
{
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = m_vGpuAddress + offset_;
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

// create views for uav and srv
void ByteAddressedBuffer::CreateDerivedView()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = (UINT)m_bufferSize / 4;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;

	if (m_srv.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_srv = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srv);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.Buffer.NumElements = (UINT)m_bufferSize / 4;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;

	if (m_uav.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_uav = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateUnorderedAccessView(m_resource.Get(), nullptr, &uavDesc, m_uav);
}

// Create views for srv and uav
void StructuredBuffer::CreateDerivedView()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = m_eCount;
	srvDesc.Buffer.StructureByteStride = m_eSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	if (m_srv.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_srv = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srv);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.NumElements = m_eCount;
	uavDesc.Buffer.StructureByteStride = m_eSize;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

	m_counterBuffer.Create("StructuredBuffer::Counter", 1, 4);

	if (m_uav.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_uav = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateUnorderedAccessView(m_resource.Get(), m_counterBuffer.GetResource(), &uavDesc, m_uav);
}

const D3D12_CPU_DESCRIPTOR_HANDLE& StructuredBuffer::GetCounterSRV(CommandContext& context_)
{
	// Transition resource via context
	return m_counterBuffer.GetSRV();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& StructuredBuffer::GetCounterUAV(CommandContext& context_)
{
	// Transition resource via context
	return m_counterBuffer.GetUAV();
}
