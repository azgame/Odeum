#include "ReadbackBuffer.h"
#include "../D3DCore.h"

void ReadbackBuffer::Create(uint32_t numElements_, uint32_t elementSize_)
{
	Destroy();

	m_eCount = numElements_;
	m_eSize = elementSize_;
	m_bufferSize = m_eCount * m_eSize;
	m_usageState = D3D12_RESOURCE_STATE_COPY_DEST;

	D3D12_HEAP_PROPERTIES props;
	props.Type = D3D12_HEAP_TYPE_READBACK;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	props.CreationNodeMask = 1;
	props.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = m_bufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	if (FAILED(DXGraphics::m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_resource))))
	{
		ERROR("Could not create read back buffer");
		throw std::runtime_error("Could not create read back buffer");
	}

#ifdef _DEBUG
	m_resource->SetName(L"Read back buffer");
#endif
}

void* ReadbackBuffer::Map()
{
	void* mem;
	m_resource->Map(0, &CD3DX12_RANGE(0, m_bufferSize), &mem);
	return mem;
}

void ReadbackBuffer::UnMap()
{
	m_resource->Unmap(0, &CD3DX12_RANGE(0, 0));
}
