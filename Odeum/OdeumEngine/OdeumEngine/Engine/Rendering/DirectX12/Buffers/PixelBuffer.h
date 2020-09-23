#ifndef PIXELBUFFER_H
#define PIXELBUFFER_H

#include "D3DResource.h"

// defines a buffer that represents a collection of pixels (width * height, possible depth). Useful for finding appropriate dxgi format (bits/bytes per pixel and type)
class PixelBuffer : public D3DResource
{
public:

	PixelBuffer() :
		m_width(0), m_height(0), m_depth(0), m_format(DXGI_FORMAT_UNKNOWN) {}

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }
	uint32_t GetDepth() const { return m_depth; }
	DXGI_FORMAT GetFormat() const { return m_format; }

	void WriteToFile(const std::string& filePath_);

protected:

	D3D12_RESOURCE_DESC CreateTextureDesc(uint32_t width_, uint32_t height_, uint32_t depthOrArraySize_, uint32_t numMips_, DXGI_FORMAT format_, UINT flags_);

	void ConnectToResource(ID3D12Device* device_, const std::wstring& name_, ID3D12Resource* resource_, D3D12_RESOURCE_STATES state_);

	void CreateTextureResource(ID3D12Device* device_, const std::wstring& name_, const D3D12_RESOURCE_DESC& desc_,
		D3D12_CLEAR_VALUE clearValue_, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN);

	// Used to get a specified format based on the given format and the function used
	static DXGI_FORMAT GetBaseFormat(DXGI_FORMAT format_);
	static DXGI_FORMAT GetUAVFormat(DXGI_FORMAT format_);
	static DXGI_FORMAT GetDSVFormat(DXGI_FORMAT format_);
	static DXGI_FORMAT GetDepthFormat(DXGI_FORMAT format_);
	static DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format_);
	static size_t BytesPerPixel(DXGI_FORMAT format_);

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depth;
	DXGI_FORMAT m_format;

};

#endif