#ifndef D3DBUFFER_H
#define D3DBUFFER_H

#include "D3DResource.h"

class CommandContext;

class D3DBuffer : public D3DResource
{
public:

	// Initial data being passed in is not copied to the buffer, can be in the future
	void Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_ = nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE ConstantBufferView(uint32_t offset_, uint32_t size_) const;
	const D3D12_CPU_DESCRIPTOR_HANDLE& UnorderedAccessView() const { return m_uav; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& ShaderResourceView() const { return m_srv; }
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t BaseVertexIndex = 0) const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t StartIndex = 0) const;

	size_t allocatedSize;
	uint32_t elementCount;
	uint32_t elementByteSize;
	D3D12_RESOURCE_FLAGS flags;

protected:

	D3DBuffer() 
		: allocatedSize(0), elementCount(0), elementByteSize(0)
	{
		flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		m_uav.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_srv.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	D3D12_RESOURCE_DESC ResourceDescription();
	virtual void CreateExtraViews() = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE m_uav;
	D3D12_CPU_DESCRIPTOR_HANDLE m_srv;
};

class ByteAddressBuffer : public D3DBuffer
{
public:
	virtual void CreateExtraViews() override;
};

class StructuredBuffer : public D3DBuffer
{
public:

	virtual void ResetResource() override
	{
		m_counterBuffer.ResetResource();
		D3DResource::ResetResource();
	}

	virtual void CreateExtraViews() override;

	ByteAddressBuffer& GetCounterBuffer() { return m_counterBuffer; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCounterSRV(CommandContext& context_);
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCounterUAV(CommandContext& context_);

private:
	ByteAddressBuffer m_counterBuffer; // if we want to read the atomic compute counter for this buffer
};

#endif