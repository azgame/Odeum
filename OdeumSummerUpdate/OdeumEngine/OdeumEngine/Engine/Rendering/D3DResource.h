#ifndef D3DRESOURCE_H
#define D3DRESOURCE_H

#include "../../pch.h" // Todo Aidan fix precompiled-ness of pch for faster inclusion in .h's, if possible

// Base class for ID3D12Resource wrapping
class D3DResource
{
public:

	D3DResource() {}
	D3DResource(ID3D12Resource* resource) {}

	virtual void Destroy()
	{
		m_pResource->Release();
	}

	ID3D12Resource* operator->() { return m_pResource; }
	const ID3D12Resource* operator->() const { return m_pResource; }

	ID3D12Resource* GetResource() { return m_pResource; }
	const ID3D12Resource* GetResource() const { return m_pResource; }

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() { return m_vGpuAddress; }
	const D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress() const { return m_vGpuAddress; }

protected:
	ID3D12Resource* m_pResource;
	D3D12_GPU_VIRTUAL_ADDRESS m_vGpuAddress;
};

#endif
