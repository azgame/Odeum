#ifndef D3DBUFFER_H
#define D3DBUFFER_H

#include "D3DResource.h"

class CommandContext;

// default Buffer. Contains: buffer size, number of elements, size of elements, relevant flags, and two cpu desc handles
class D3DBuffer : public D3DResource
{
public:

	// Initial data being passed in is not copied to the buffer, can be in the future
	void Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_ = nullptr);

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const { return m_uav; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_srv; }

	const D3D12_GPU_VIRTUAL_ADDRESS& GetRootCBV() const { return m_gpuAddress; }

	D3D12_CPU_DESCRIPTOR_HANDLE CreateCBV(uint32_t offset_, uint32_t size_) const;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t offset_, uint32_t size_, uint32_t stride_) const;
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t BaseVertexIndex = 0) const
	{
		size_t offset = BaseVertexIndex * m_eSize;
		return VertexBufferView(offset, (uint32_t)(m_bufferSize - offset), m_eSize);
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t offset_, uint32_t size_, bool b32Bit = false) const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t StartIndex = 0) const
	{
		size_t offset = StartIndex * m_eSize;
		return IndexBufferView(offset, (uint32_t)(m_bufferSize - offset), m_eSize == 4);
	}

	size_t GetBufferSize() { return m_bufferSize; }
	uint32_t GetElementCount() { return m_eCount; }
	uint32_t GetElementSize() { return m_eSize; }

protected:

	D3DBuffer() : m_bufferSize(0), m_eCount(0), m_eSize(0)
	{
		m_resourceFlags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		m_uav.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
		m_srv.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
	}

	D3D12_RESOURCE_DESC CreateBufferDescription();
	virtual void CreateDerivedView() = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE m_uav;
	D3D12_CPU_DESCRIPTOR_HANDLE m_srv;

	size_t m_bufferSize;
	uint32_t m_eCount;
	uint32_t m_eSize;

	D3D12_RESOURCE_FLAGS m_resourceFlags;
};

class ByteAddressedBuffer : public D3DBuffer
{
public:
	virtual void CreateDerivedView() override;
};

class StructuredBuffer : public D3DBuffer
{
public:

	virtual void Destroy() override
	{
		m_counterBuffer.Destroy();
		D3DResource::Destroy();
	}

	virtual void CreateDerivedView() override;

	ByteAddressedBuffer& GetCounterBuffer() { return m_counterBuffer; }

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCounterSRV(CommandContext& context_);
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetCounterUAV(CommandContext& context_);

private:
	ByteAddressedBuffer m_counterBuffer; // if we want to read the atomic compute counter for this buffer
};

#endif