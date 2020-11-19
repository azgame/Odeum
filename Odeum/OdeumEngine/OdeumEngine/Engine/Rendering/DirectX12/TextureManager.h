#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "D3DIncludes.h"
#include "Buffers/D3DResource.h"

#include <map>

class Colour;

class Texture : public D3DResource
{
	friend class CommandContext;

public:

	Texture(const std::string& fileName_) 
	{
		m_cpuDescHandle.ptr = D3D12_GPU_VIRTUAL_ADDRESS_NULL;
		m_isValid = true;
	}

	~Texture()
	{
		m_cpuDescHandle.ptr = 0;
	}

	Texture(const std::string& fileName_, D3D12_CPU_DESCRIPTOR_HANDLE handle) : m_cpuDescHandle(handle), m_isValid(true) {}

	void Create(size_t pitch_, size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_);
	void Create(size_t width_, size_t height_, DXGI_FORMAT format_, const void* initialData_)
	{
		Create(width_, width_, height_, format_, initialData_);
	}

	virtual void ResetResource() override
	{
		D3DResource::ResetResource();
		//m_cpuDescHandle.ptr = 0;
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const { return m_cpuDescHandle; }

	void SetToInvalidTexture();
	bool isValid() const { return m_isValid; }

private:

	bool m_isValid;
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

	void Initialize(std::string textureDirectory_) 
	{ 
		sm_rootDirectory = textureDirectory_; 
		GetInvalidTexture();
	}

	void ShutDown() 
	{
		sm_textureMap.clear(); 
	}

	Texture* LoadFromFile(std::string textureName_);
	Texture* CreateAndStore(Colour colour_);

	Texture* GetInvalidTexture();

private:

	struct FormattedRawTexture
	{
		UINT8* formattedData;
		int width = 0;
		int height = 0;
		int stride = 0;
		int offset = 0;
	};

	TextureManager() {}
	~TextureManager() { ShutDown(); }
	static std::unique_ptr<TextureManager> m_instance;
	friend std::default_delete<TextureManager>;

	void FormatTexture(FormattedRawTexture& tex, UINT8* pixels, DXGI_FORMAT format);
	Texture* FindOrLoad(std::string textureName_);
	
	std::map<std::string, std::unique_ptr<Texture>> sm_textureMap;
	std::mutex sm_mutex;
	std::string sm_rootDirectory;
};

#endif