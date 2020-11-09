#include "D3DBuffer.h"

#include "../D3DCore.h"
#include "../CommandContext.h"

using namespace DXGraphics;

void D3DBuffer::Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_)
{
	ResetResource();

	elementCount = numElements_;
	elementByteSize = elementSize_;
	allocatedSize = numElements_ * elementSize_;
	currentState = D3D12_RESOURCE_STATE_COMMON;

	D3D12_RESOURCE_DESC desc = ResourceDescription();

	if (FAILED(DXGraphics::m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &desc, currentState, nullptr, IID_PPV_ARGS(&m_resource))))
		Debug::Error("Could not create buffer", __FILENAME__, __LINE__);

	m_gpuAddress = m_resource->GetGPUVirtualAddress();
	m_resource->SetName(Utility::MakeWStr(name_).c_str());

	if (initialData_)
		CommandContext::InitializeBuffer(*this, initialData_, allocatedSize);

	CreateExtraViews();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DBuffer::ConstantBufferView(uint32_t offset_, uint32_t size_) const
{
	if (offset_ + size_ > allocatedSize)
		Debug::Error("Attempted allocation of descriptor failed!", __FILENAME__, __LINE__);

	size_ = Utility::Align(size_, 16);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = offset_;
	cbvDesc.SizeInBytes = size_;

	D3D12_CPU_DESCRIPTOR_HANDLE cbv = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateConstantBufferView(&cbvDesc, cbv);

	return cbv;
}

D3D12_VERTEX_BUFFER_VIEW D3DBuffer::VertexBufferView(size_t BaseVertexIndex) const
{
	size_t offset = BaseVertexIndex * elementByteSize;
	size_t size = (uint32_t)(allocatedSize - offset);

	D3D12_VERTEX_BUFFER_VIEW vertBufferView;
	vertBufferView.BufferLocation = m_gpuAddress + offset;
	vertBufferView.SizeInBytes = size;
	vertBufferView.StrideInBytes = elementByteSize;
	return vertBufferView;
}

D3D12_INDEX_BUFFER_VIEW D3DBuffer::IndexBufferView(size_t StartIndex) const
{
	size_t offset = StartIndex * elementByteSize;
	size_t size = (uint32_t)(allocatedSize - offset);
	bool b32bit = elementByteSize == 4;

	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = m_gpuAddress + offset;
	indexBufferView.SizeInBytes = size;
	indexBufferView.Format = b32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
	return indexBufferView;
}

D3D12_RESOURCE_DESC D3DBuffer::ResourceDescription()
{
	D3D12_RESOURCE_DESC Desc = {};
	Desc.Alignment = 0;
	Desc.DepthOrArraySize = 1;
	Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	Desc.Flags = flags;
	Desc.Format = DXGI_FORMAT_UNKNOWN;
	Desc.Height = 1;
	Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	Desc.MipLevels = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Width = (UINT64)allocatedSize;
	return Desc;
}

// create views for uav and srv
void ByteAddressedBuffer::CreateExtraViews()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = (UINT)allocatedSize / 4;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;

	if (m_srv.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_srv = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srv);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.Buffer.NumElements = (UINT)allocatedSize / 4;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;

	if (m_uav.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_uav = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateUnorderedAccessView(m_resource.Get(), nullptr, &uavDesc, m_uav);
}

// Create views for srv and uav
void StructuredBuffer::CreateExtraViews()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = elementCount;
	srvDesc.Buffer.StructureByteStride = elementByteSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	if (m_srv.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_srv = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srv);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.NumElements = elementCount;
	uavDesc.Buffer.StructureByteStride = elementByteSize;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

	m_counterBuffer.Create("StructuredBuffer::Counter", 1, 4);

	if (m_uav.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_uav = AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DXGraphics::m_device->CreateUnorderedAccessView(m_resource.Get(), m_counterBuffer.GetResource(), &uavDesc, m_uav);
}

const D3D12_CPU_DESCRIPTOR_HANDLE& StructuredBuffer::GetCounterSRV(CommandContext& context_)
{
	context_.TransitionResource(m_counterBuffer, D3D12_RESOURCE_STATE_GENERIC_READ);
	return m_counterBuffer.ShaderResourceView();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& StructuredBuffer::GetCounterUAV(CommandContext& context_)
{
	context_.TransitionResource(m_counterBuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	return m_counterBuffer.UnorderedAccessView();
}
