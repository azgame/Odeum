#ifndef D3DBUFFER_H
#define D3DBUFFER_H

#include "../../pch.h"

#include "D3DResource.h"


class D3DBuffer : public D3DResource
{
public:

	// Initial data being passed in is not copied to the buffer, should be in future
	void Create(std::string name_, uint32_t numElements_, uint32_t elementSize_, const void* initialData_ = nullptr);

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const { return m_uav; }
	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_srv; }

	const D3D12_GPU_VIRTUAL_ADDRESS& GetRootCBV() const { return m_vGpuAddress; }

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

	D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV(GraphicsContext& context_);
	D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV(GraphicsContext& context_);

private:
	ByteAddressedBuffer m_counterBuffer; // This is in case we want to use our own uav counter buffer rather than the driver's, which is faster and easier
										// but, we only need this for reading atomic counters, so only really used for indirect drawing in some contexts (moving cpu work to gpu)
};

#endif