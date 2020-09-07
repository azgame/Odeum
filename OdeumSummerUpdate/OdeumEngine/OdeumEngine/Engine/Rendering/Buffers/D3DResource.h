#ifndef D3DRESOURCE_H
#define D3DRESOURCE_H

#include "../../../pch.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL 0ull
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN ~0ull

class GraphicsContext;

// Base class for ID3D12Resource wrapping
class D3DResource
{
public:

	D3DResource() : 
		m_vGpuAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL), 
		m_resource(nullptr),
		m_usageState(D3D12_RESOURCE_STATE_COMMON)
	{}

	D3DResource(ID3D12Resource* resource_, D3D12_RESOURCE_STATES state_) : 
		m_resource(resource_), 
		m_usageState(state_) 
	{
		m_vGpuAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	}

	virtual void Destroy()
	{
		m_resource->Release();
	}

	ID3D12Resource* operator->() { return m_resource; }
	const ID3D12Resource* operator->() const { return m_resource; }

	ID3D12Resource* GetResource() { return m_resource; }
	const ID3D12Resource* GetResource() const { return m_resource; }

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() { return m_vGpuAddress; }
	const D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_vGpuAddress; }

protected:
	ID3D12Resource* m_resource;
	D3D12_GPU_VIRTUAL_ADDRESS m_vGpuAddress;
	D3D12_RESOURCE_STATES m_usageState;
};

#endif
