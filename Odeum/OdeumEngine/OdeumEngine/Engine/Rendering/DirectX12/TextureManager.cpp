#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h"

#include <sstream>

#include "D3DCore.h"
#include "CommandContext.h"
#include "Colour.h"
#include "RadiometryUtility.h"

std::unique_ptr<TextureManager> TextureManager::m_instance = nullptr;

TextureManager* TextureManager::Get()
{
	if (m_instance.get() == nullptr)
		m_instance.reset(new TextureManager);
	return m_instance.get();
}

void Texture::Create(size_t pitch_, size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_)
{
    ResetResource();

    currentState = D3D12_RESOURCE_STATE_COPY_DEST;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width = (UINT)width_;
    desc.Height = (UINT)height_;
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
        &desc, currentState, nullptr, IID_PPV_ARGS(&m_resource))))
    {
        ERROR("Could not create texture resource!");
        ASSERT(false, "Texture creation failed!");
    }

    D3D12_SUBRESOURCE_DATA subresource;
    subresource.pData = initialData_;
    subresource.RowPitch = pitch_ * DXUtility::BytesPerPixel(format_);
    subresource.SlicePitch = subresource.RowPitch * height_;

    CommandContext::InitializeTexture(*this, 1, &subresource);

    if (m_cpuDescHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_NULL)
        m_cpuDescHandle = DXGraphics::AllocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    DXGraphics::m_device->CreateShaderResourceView(m_resource.Get(), nullptr, m_cpuDescHandle);
}

void Texture::SetToInvalidTexture()
{
    m_cpuDescHandle = TextureManager::Get()->GetInvalidTexture()->GetSRV();
    m_isValid = false;
}

// Seperating initial loading call from possible find so that we can eventually decide when to put a texture into memory
Texture* TextureManager::LoadFromFile(std::string textureName_)
{
	Texture* texture = FindOrLoad(textureName_);
	return texture;
}

// NYI
Texture* TextureManager::LoadColour(Colour colour_)
{
    Texture* texture = GetColourTexture(colour_);
    return texture;
}

void TextureManager::FormatTexture(FormattedRawTexture& tex, UINT8* pixels, DXGI_FORMAT format)
{
	const UINT numPixels = tex.width * tex.height;
	const UINT oldStride = tex.stride;
	const UINT oldSize = numPixels * tex.stride;

	const UINT newStride = DXUtility::BytesPerPixel(format);
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
    // newTexture->SetToInvalidTexture();

	FormattedRawTexture result = {};
	UINT8* initialData = stbi_load((sm_rootDirectory + textureName_).c_str(), &result.width, &result.height, &result.stride, STBI_default);

    sm_textureMap[textureName_].reset(newTexture);

    if (initialData)
    {
        FormatTexture(result, initialData, DXGI_FORMAT_R8G8B8A8_UNORM);
        newTexture->Create(result.width, result.height, DXGI_FORMAT_R8G8B8A8_UNORM, result.formattedData);
    }
    else
    {
        newTexture->SetToInvalidTexture();
    }
    
    //SAFE_DELETE(initialData);
	return newTexture;
}

Texture* TextureManager::GetColourTexture(Colour colour)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << colour.GetR() << colour.GetG() << colour.GetB() << colour.GetA();
    std::string colourName = stream.str();

    Texture* colourTexture;

    auto iter = sm_textureMap.find(colourName);
    if (iter != sm_textureMap.end())
    {
        colourTexture = iter->second.get();
        return colourTexture;
    }
    else
    {
        sm_mutex.lock();
        colourTexture = new Texture(colourName);
        sm_textureMap[colourName].reset(colourTexture);
        sm_mutex.unlock();
    }

    colourTexture->Create(1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, colour.GetPtr());
    return colourTexture;
}

Texture* TextureManager::GetInvalidTexture()
{
    Texture* invalid;

    auto iter = sm_textureMap.find("Invalid");
    if (iter != sm_textureMap.end())
    {
        invalid = iter->second.get();
        return invalid;
    }
    else
    {
        sm_mutex.lock();
        invalid = new Texture("Invalid");
        sm_textureMap["Invalid"].reset(invalid);
        sm_mutex.unlock();
    }

    uint32_t MagentaPixel = 0x00FF00FF;
    invalid->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &MagentaPixel);
    return invalid;
}




