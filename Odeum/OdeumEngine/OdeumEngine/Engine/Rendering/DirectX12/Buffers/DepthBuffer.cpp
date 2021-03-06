#include "DepthBuffer.h"

#include "../D3DCore.h"
#include "../RadiometryUtility.h"

// Accessible interface to create a depth buffer. Create texture resource creates a commited texture resource. Real work done in create derived views
void DepthBuffer::Create(const std::wstring& name_, uint32_t width_, uint32_t height_, DXGI_FORMAT format_, uint32_t numSamples_, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_)
{
	D3D12_RESOURCE_DESC desc = CreateTextureDesc(width_, height_, 1, 1, format_, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	desc.SampleDesc.Count = numSamples_;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format_;
	CreateTextureResource(DXGraphics::m_device, name_, desc, clearValue, gpuAddress_);
	CreateDerivedViews(DXGraphics::m_device, format_);
}

void DepthBuffer::CreateDerivedViews(ID3D12Device* device_, DXGI_FORMAT format_)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXUtility::GetDSVFormat(format_);

	if (m_resource->GetDesc().SampleDesc.Count == 1)
	{
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
	}
	else
	{
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
	}

	// DSV/Depth
	// Allocate descriptors for dsv and depth read only
	if (m_dsvHandle[0].ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		m_dsvHandle[0] = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_dsvHandle[1] = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	device_->CreateDepthStencilView(m_resource.Get(), &dsvDesc, m_dsvHandle[0]); // create dsv that is written to

	dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
	device_->CreateDepthStencilView(m_resource.Get(), &dsvDesc, m_dsvHandle[1]); // create dsv that is written to

	DXGI_FORMAT stencilReadFormat = DXUtility::GetStencilFormat(format_);
	if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
	{
		if (m_dsvHandle[2].ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		{
			m_dsvHandle[2] = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_dsvHandle[3] = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}

		dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
		device_->CreateDepthStencilView(m_resource.Get(), &dsvDesc, m_dsvHandle[2]);

		dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
		device_->CreateDepthStencilView(m_resource.Get(), &dsvDesc, m_dsvHandle[3]);
	}
	else
	{
		m_dsvHandle[2] = m_dsvHandle[0];
		m_dsvHandle[3] = m_dsvHandle[1];
	}

	// Create depth shader resource view
	if (m_depthSRVHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
		m_depthSRVHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXUtility::GetDepthFormat(format_);

	if (dsvDesc.ViewDimension == D3D12_DSV_DIMENSION_TEXTURE2D)
	{
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
	}
	else
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;

	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	device_->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_depthSRVHandle);

	// Create stencil shader resource view
	if (stencilReadFormat != DXGI_FORMAT_UNKNOWN)
	{
		if (m_stencilSRVHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
			m_stencilSRVHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		srvDesc.Format = stencilReadFormat;
		device_->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_stencilSRVHandle);
	}
}
