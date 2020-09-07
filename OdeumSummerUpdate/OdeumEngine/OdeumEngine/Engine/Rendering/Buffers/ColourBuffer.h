#ifndef COLOURBUFFER_H
#define COLOURBUFFER_H

#include "PixelBuffer.h"
#include "../Colour.h"

// TODO Aidan: Make comments

class ColourBuffer : public PixelBuffer
{
public:

    ColourBuffer(Colour clearColour = Colour(0.0f, 0.0f, 0.0f, 0.0f)) :
        m_clearColour(clearColour), m_numMipMaps(0), m_fragmentCount(1), m_sampleCount(1)
    {
        m_srvHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        m_rtvHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN;
        ZeroMemory(m_uavHandle, sizeof(m_uavHandle));
    }

    void CreateFromSwapChain(const std::wstring& name_, ID3D12Resource* pResource_);

    void Create(const std::wstring& name_, uint32_t width_, uint32_t height_, uint32_t numMips_,
        DXGI_FORMAT format_, D3D12_GPU_VIRTUAL_ADDRESS vmPtr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

    void CreateArray(const std::wstring& name_, uint32_t width_, uint32_t height_, uint32_t arrayCount_,
        DXGI_FORMAT format_, D3D12_GPU_VIRTUAL_ADDRESS vmPtr = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

    const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_srvHandle; }
    const D3D12_CPU_DESCRIPTOR_HANDLE& GetRTV() const { return m_rtvHandle; }
    const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const { return m_uavHandle[0]; }

    void SetMsaaMode(uint32_t numColorSamples_, uint32_t numCoverageSamples_)
    {
        assert(numCoverageSamples_ >= numColorSamples_);
        m_fragmentCount = numColorSamples_;
        m_sampleCount = numCoverageSamples_;
    }

    void SetClearColour(Colour clearColour_) { m_clearColour = clearColour_; }
    Colour GetClearColour() const { return m_clearColour; }
    
    void GenerateMipMaps(GraphicsContext& context_);

protected:

    D3D12_RESOURCE_FLAGS CombineResourceFlags() const
    {
        D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

        if (m_fragmentCount == 1)
            flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

        return D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | flags;
    }

    static inline uint32_t CalculateNumMips(uint32_t width_, uint32_t height_)
    {
        uint32_t hBit;
        _BitScanReverse((unsigned long*)&hBit, width_ | height_);
        return hBit + 1;
    }

    void CreateDerivedViews(ID3D12Device* device_, DXGI_FORMAT format_, uint32_t arraySize_, uint32_t numMips_ = 1);

    Colour m_clearColour;
    D3D12_CPU_DESCRIPTOR_HANDLE m_srvHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;
    D3D12_CPU_DESCRIPTOR_HANDLE m_uavHandle[12];
    uint32_t m_numMipMaps;
    uint32_t m_fragmentCount;
    uint32_t m_sampleCount;
};

#endif