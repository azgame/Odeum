#include "PixelBuffer.h"
#include "../RadiometryUtility.h"

void PixelBuffer::WriteToFile(const std::string& filePath_)
{
    // Need readback buffers first
}

// Standard description creation
D3D12_RESOURCE_DESC PixelBuffer::CreateTextureDesc(uint32_t width_, uint32_t height_, uint32_t depthOrArraySize_, uint32_t numMips_, DXGI_FORMAT format_, UINT flags_)
{
    m_width = width_;
    m_height = height_;
    m_depth = depthOrArraySize_;
    m_format = format_;

    D3D12_RESOURCE_DESC desc = {};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.DepthOrArraySize = m_depth;
    desc.MipLevels = numMips_;
    desc.Format = DXUtility::GetBaseFormat(m_format);
    desc.Alignment = 0;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Flags = (D3D12_RESOURCE_FLAGS)flags_;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    return desc;
}

// Essentially copying the memory address and values of a given resource
void PixelBuffer::ConnectToResource(ID3D12Device* device_, const std::wstring& name_, ID3D12Resource* resource_, D3D12_RESOURCE_STATES state_)
{
    ASSERT(resource_ != nullptr, "Can't connect to resource if its null");

    D3D12_RESOURCE_DESC desc = resource_->GetDesc();
    m_resource = resource_;
    currentState = state_;

    m_width = (uint32_t)desc.Width;
    m_height = desc.Height;
    m_depth = desc.DepthOrArraySize;
    m_format = desc.Format;

    m_resource->SetName(name_.c_str());
}

// Reset and recreate resource based on given desc. Can use given gpuAddress in future to place resource instead of creating new
void PixelBuffer::CreateTextureResource(ID3D12Device* device_, const std::wstring& name_, const D3D12_RESOURCE_DESC& desc_, D3D12_CLEAR_VALUE clearValue_, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_)
{
    ResetResource();

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

    if (FAILED(device_->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc_,
        D3D12_RESOURCE_STATE_COMMON, &clearValue_, IID_PPV_ARGS(&m_resource))))
        Debug::Error("Failed to create texture resource", __FILENAME__, __LINE__);

    currentState = D3D12_RESOURCE_STATE_COMMON;
    m_gpuAddress = D3D12_GPU_VIRTUAL_ADDRESS_NULL;

    m_resource->SetName(name_.c_str());
}