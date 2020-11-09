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
		currentState(D3D12_RESOURCE_STATE_COMMON),
		transitionState((D3D12_RESOURCE_STATES)-1)
	{}

	D3DResource(ID3D12Resource* resource_, D3D12_RESOURCE_STATES state_) : 
		m_gpuAddress(D3D12_GPU_VIRTUAL_ADDRESS_NULL),
		m_resource(resource_), 
		currentState(state_),
		transitionState((D3D12_RESOURCE_STATES)-1)
	{}

	virtual void ResetResource()
	{
		m_gpuAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
		m_resource = nullptr;
	}

	ID3D12Resource* GetResource() { return m_resource.Get(); }
	const ID3D12Resource* GetResource() const { return m_resource.Get(); }

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() { return m_gpuAddress; }
	const D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_gpuAddress; }

	D3D12_RESOURCE_STATES currentState;
	D3D12_RESOURCE_STATES transitionState;

protected:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
	D3D12_GPU_VIRTUAL_ADDRESS m_gpuAddress;
};



#endif