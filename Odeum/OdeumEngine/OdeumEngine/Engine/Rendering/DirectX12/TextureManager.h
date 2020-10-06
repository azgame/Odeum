#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "D3DIncludes.h"
#include "Buffers/D3DResource.h"

class Texture : public D3DResource
{
	friend class CommandContext;

public:

	Texture(const std::string& fileName_) 
	{
		m_textureKey = fileName_;
		m_cpuDescHandle.ptr == D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN; 
	}

	Texture(const std::string& fileName_, D3D12_CPU_DESCRIPTOR_HANDLE handle) : m_cpuDescHandle(handle), m_textureKey(fileName_) {}

	void Create(size_t pitch_, size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_);
	void Create(size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_)
	{
		Create(width_, width_, height_, format_, initialData_);
	}

	void CreateFromMemory(const void* bufferEntry_, size_t fileSize_);

	virtual void Destroy() override
	{
		D3DResource::Destroy();
		m_cpuDescHandle.ptr = 0;
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_cpuDescHandle; }

private:

	std::string m_textureKey;
	D3D12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandle;
};

class TextureManager
{
public:
	static TextureManager* Get();
	TextureManager(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;


};

#endif