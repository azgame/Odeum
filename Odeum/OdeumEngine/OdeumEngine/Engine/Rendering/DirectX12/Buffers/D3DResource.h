#ifndef D3DRESOURCE_H
#define D3DRESOURCE_H

#include "../D3DIncludes.h"

class GraphicsContext;

// Base class for ID3D12Resource wrapping
class D3DResource
{
public:

	D3DResource() : 
		m_gpuAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL), 
		m_resource(nullptr),
		m_usageState(D3D12_RESOURCE_STATE_COMMON),
		m_transitioningState((D3D12_RESOURCE_STATES)-1)
	{}

	D3DResource(ID3D12Resource* resource_, D3D12_RESOURCE_STATES state_) : 
		m_resource(resource_), 
		m_usageState(state_),
		m_transitioningState((D3D12_RESOURCE_STATES)-1)
	{
		m_gpuAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
	}

	virtual ~D3DResource()
	{
		Destroy();
	}

	virtual void Destroy()
	{
		m_gpuAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
		m_resource.Reset();
	}

	ID3D12Resource* operator->() { return m_resource.Get(); }
	const ID3D12Resource* operator->() const { return m_resource.Get(); }

	ID3D12Resource* GetResource() { return m_resource.Get(); }
	const ID3D12Resource* GetResource() const { return m_resource.Get(); }

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() { return m_gpuAddress; }
	const D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_gpuAddress; }

	D3D12_RESOURCE_STATES m_usageState;
	D3D12_RESOURCE_STATES m_transitioningState;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuAddress;
};

#endif