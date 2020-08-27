#ifndef COLOURBUFFER_H
#define COLOURBUFFER_H

#include "PixelBuffer.h"
#include "Colour.h"

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



    Colour GetClearColour() const { return m_clearColour; }
    
    void GenerateMipMaps(GraphicsContext& context_);

protected:



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
    uint32_t m_numMipMaps; // number of texture sublevels
    uint32_t m_fragmentCount;
    uint32_t m_sampleCount;
};

#endif