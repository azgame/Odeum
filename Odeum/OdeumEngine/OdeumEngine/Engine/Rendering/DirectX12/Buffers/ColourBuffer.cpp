#include "ColourBuffer.h"

#include "../D3DCore.h"
#include "../RadiometryUtility.h"

void ColourBuffer::CreateFromSwapChain(const std::wstring& name_, ID3D12Resource* pResource_)
{
	ConnectToResource(DXGraphics::m_device, name_, pResource_, D3D12_RESOURCE_STATE_PRESENT);

	m_rtvHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	m_resource->Release();
	DXGraphics::m_device->CreateRenderTargetView(m_resource.Get(), nullptr, m_rtvHandle);
}

// Wholesale creation of buffer. Determine num mips, set flags, create texture desc using num mip maps. Then set clear colour and create the texture resource and call any derived functions at the end
void ColourBuffer::Create(const std::wstring& name_, uint32_t width_, uint32_t height_, uint32_t numMips_, DXGI_FORMAT format_, D3D12_GPU_VIRTUAL_ADDRESS vmPtr)
{
	numMips_ = (numMips_ == 0 ? CalculateNumMips(width_, height_) : numMips_);
	D3D12_RESOURCE_FLAGS flags = CombineResourceFlags(); 
	D3D12_RESOURCE_DESC desc = CreateTextureDesc(width_, height_, 1, numMips_, format_, flags);

	desc.SampleDesc.Count = m_fragmentCount;
	desc.SampleDesc.Quality = 0;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format_;
	clearValue.Color[0] = m_clearColour.GetR();
	clearValue.Color[1] = m_clearColour.GetG();
	clearValue.Color[2] = m_clearColour.GetB();
	clearValue.Color[3] = m_clearColour.GetA();

	CreateTextureResource(DXGraphics::m_device, name_, desc, clearValue, vmPtr);
	CreateDerivedViews(DXGraphics::m_device, format_, 1, numMips_);
}

// Wholesale creation of buffer. Set flags, create texture desc as a texture array. Then set clear colour and create the texture resource and call any derived functions at the end
void ColourBuffer::CreateArray(const std::wstring& name_, uint32_t width_, uint32_t height_, uint32_t arrayCount_, DXGI_FORMAT format_, D3D12_GPU_VIRTUAL_ADDRESS vmPtr)
{
	D3D12_RESOURCE_FLAGS flags = CombineResourceFlags();
	D3D12_RESOURCE_DESC desc = CreateTextureDesc(width_, height_, arrayCount_, 1, format_, flags);

	desc.SampleDesc.Count = m_fragmentCount;
	desc.SampleDesc.Quality = 0;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format_;
	clearValue.Color[0] = m_clearColour.GetR();
	clearValue.Color[1] = m_clearColour.GetG();
	clearValue.Color[2] = m_clearColour.GetB();
	clearValue.Color[3] = m_clearColour.GetA();

	CreateTextureResource(DXGraphics::m_device, name_, desc, clearValue, vmPtr);
	CreateDerivedViews(DXGraphics::m_device, format_, arrayCount_, 1);
}

void ColourBuffer::CreateDerivedViews(ID3D12Device* device_, DXGI_FORMAT format_, uint32_t arraySize_, uint32_t numMips_)
{
	ASSERT(arraySize_ == 1 || numMips_ == 1, "Only one of array size or num mips must equal 0!");

	m_numMipMaps = numMips_;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	rtvDesc.Format = format_;
	uavDesc.Format = DXUtility::GetUAVFormat(format_);
	srvDesc.Format = format_;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// is it a texture array or do we have mipmaps?
	if (arraySize_ > 1)
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.ArraySize = (UINT)arraySize_;

		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
		uavDesc.Texture2DArray.MipSlice = 0;
		uavDesc.Texture2DArray.FirstArraySlice = 0;
		uavDesc.Texture2DArray.ArraySize = (UINT)arraySize_;

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MipLevels = numMips_;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = (UINT)arraySize_;
	}
	else if (m_fragmentCount > 1)
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = numMips_;
		srvDesc.Texture2D.MostDetailedMip = 0;
	}

	if (m_srvHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
	{
		m_rtvHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_srvHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	device_->CreateRenderTargetView(m_resource.Get(), &rtvDesc, m_rtvHandle);
	device_->CreateShaderResourceView(m_resource.Get(), &srvDesc, m_srvHandle);

	if (m_fragmentCount > 1)
		return;

	// uavs for each mip level
	for (uint32_t i = 0; i < numMips_; i++)
	{
		if (m_uavHandle[i].ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
			m_uavHandle[i] = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		device_->CreateUnorderedAccessView(m_resource.Get(), nullptr, &uavDesc, m_uavHandle[i]);

		uavDesc.Texture2D.MipSlice++;
	}
}
