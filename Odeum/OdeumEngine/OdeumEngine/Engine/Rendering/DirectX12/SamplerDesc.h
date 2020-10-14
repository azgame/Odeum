#ifndef SAMPLERDESC_H
#define SAMPLERDESC_H

#include "../../../pch.h"
#include "Colour.h"

class SamplerDesc : public D3D12_SAMPLER_DESC
{
public:
    SamplerDesc()
    {
        Filter = D3D12_FILTER_ANISOTROPIC;
        AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        MipLODBias = 0.0f;
        MaxAnisotropy = 16;
        ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        BorderColor[0] = 1.0f;
        BorderColor[1] = 1.0f;
        BorderColor[2] = 1.0f;
        BorderColor[3] = 1.0f;
        MinLOD = 0.0f;
        MaxLOD = D3D12_FLOAT32_MAX;
    }

    void SetTextureAddressMode(D3D12_TEXTURE_ADDRESS_MODE AddressMode)
    {
        AddressU = AddressMode;
        AddressV = AddressMode;
        AddressW = AddressMode;
    }

    void SetBorderColor(Colour Border)
    {
        BorderColor[0] = Border.GetR();
        BorderColor[1] = Border.GetG();
        BorderColor[2] = Border.GetB();
        BorderColor[3] = Border.GetA();
    }
};

#endif