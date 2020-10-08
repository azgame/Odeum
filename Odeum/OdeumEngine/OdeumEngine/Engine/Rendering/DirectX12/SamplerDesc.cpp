#include "SamplerDesc.h"
#include "D3DCore.h"

D3D12_CPU_DESCRIPTOR_HANDLE SamplerDesc::CreateDescriptor()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	DXGraphics::m_device->CreateSampler(this, handle);
	return handle;
}
