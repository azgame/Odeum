#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h"

#include "D3DCore.h"
#include "CommandContext.h"
#include "Colour.h"

std::unique_ptr<TextureManager> TextureManager::m_instance = nullptr;

TextureManager* TextureManager::Get()
{
	if (m_instance.get() == nullptr)
		m_instance.reset(new TextureManager);
	return m_instance.get();
}

void Texture::Create(size_t pitch_, size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_)
{
    m_usageState = D3D12_RESOURCE_STATE_COPY_DEST;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = width_;
    desc.Height = height_;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = format_;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES heapProps;
    heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProps.CreationNodeMask = 1;
    heapProps.VisibleNodeMask = 1;

    if (FAILED(DXGraphics::m_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
        &desc, m_usageState, nullptr, IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf()))))
    {
        ERROR("Could not create texture resource!");
        ASSERT(false, "Texture creation failed!");
    }

    D3D12_SUBRESOURCE_DATA subresource;
    subresource.pData = initialData_;
    subresource.RowPitch = pitch_ * TextureManager::Get()->BytesPerPixel(format_);
    subresource.SlicePitch = subresource.RowPitch * height_;

    CommandContext::InitializeTexture(*this, 1, &subresource);

    if (m_cpuDescHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN)
        m_cpuDescHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), nullptr, m_cpuDescHandle);
}

void Texture::SetToInvalidTexture()
{
    uint32_t MagentaPixel = 0x00FF00FF;
    Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &MagentaPixel);
    m_isValid = false;
}

// Seperating initial loading call from possible find so that we can eventually decide when to put a texture into memory
Texture* TextureManager::LoadFromFile(std::string textureName_)
{
	Texture* texture = FindOrLoad(textureName_);
	return texture;
}

Texture* TextureManager::CreateAndStore(Colour colour_)
{
    return new Texture("Empty");
}

void TextureManager::FormatTexture(FormattedRawTexture& tex, UINT8* pixels, DXGI_FORMAT format)
{
	const UINT numPixels = tex.width * tex.height;
	const UINT oldStride = tex.stride;
	const UINT oldSize = numPixels * tex.stride;

	const UINT newStride = BytesPerPixel(format);
	const UINT newSize = numPixels * newStride;
	tex.formattedData = new UINT8[newSize];

	for (UINT i = 0; i < numPixels; i++)
	{
		tex.formattedData[i * newStride + 0] = pixels[i * oldStride + 0];
		tex.formattedData[i * newStride + 1] = pixels[i * oldStride + 1];
		tex.formattedData[i * newStride + 2] = pixels[i * oldStride + 2];
		tex.formattedData[i * newStride + 3] = 0xFF;
	}

	tex.stride = newStride;
}

// Seperate function for threaded portion (because of mutex RAII)
Texture* TextureManager::FindOrLoad(std::string textureName_)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	auto iter = sm_textureMap.find(textureName_);
	if (iter != sm_textureMap.end())
	{
		return iter->second.get();
	}

	Texture* newTexture = new Texture(textureName_);
	sm_textureMap[textureName_].reset(newTexture);

	FormattedRawTexture result = {};
	UINT8* initialData = stbi_load((sm_rootDirectory + textureName_).c_str(), &result.width, &result.height, &result.stride, STBI_default);
	
    if (initialData)
    {
        FormatTexture(result, initialData, DXGI_FORMAT_R8G8B8A8_UNORM);
        newTexture->Create(result.width, result.height, DXGI_FORMAT_R8G8B8A8_UNORM, result.formattedData);
    }
    else
    {
        newTexture->SetToInvalidTexture();
    }
    
    SAFE_DELETE(initialData);
	return newTexture;
}

size_t TextureManager::BitsPerPixel(DXGI_FORMAT format)
{
    switch (format)
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case DXGI_FORMAT_Y416:
    case DXGI_FORMAT_Y210:
    case DXGI_FORMAT_Y216:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    case DXGI_FORMAT_AYUV:
    case DXGI_FORMAT_Y410:
    case DXGI_FORMAT_YUY2:
        return 32;

    case DXGI_FORMAT_P010:
    case DXGI_FORMAT_P016:
        return 24;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
    case DXGI_FORMAT_A8P8:
    case DXGI_FORMAT_B4G4R4A4_UNORM:
        return 16;

    case DXGI_FORMAT_NV12:
    case DXGI_FORMAT_420_OPAQUE:
    case DXGI_FORMAT_NV11:
        return 12;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
    case DXGI_FORMAT_AI44:
    case DXGI_FORMAT_IA44:
    case DXGI_FORMAT_P8:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
        return 0;
    }
}

UINT TextureManager::BytesPerPixel(DXGI_FORMAT format)
{
    return (UINT)BitsPerPixel(format) / 8;
}