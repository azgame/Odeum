#include "TextureHandler.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION

std::unique_ptr<TextureHandler> TextureHandler::m_instance = nullptr;

TextureHandler::TextureHandler() {}


TextureHandler::~TextureHandler()
{
	Uninitialize();
}


void TextureHandler::FormatTexture(Texture& info, UINT8* pixels)
{
	const UINT numPixels = (info.width * info.height);
	const UINT oldStride = info.stride;
	const UINT oldSize = (numPixels * info.stride);

	const UINT newStride = 4; // uploading textures to GPU as DXGI_FORMAT_R8G8B8A8_UNORM
	const UINT newSize = (numPixels * newStride);
	info.pixels.resize(newSize);

	for (UINT i = 0; i < numPixels; i++)
	{
		info.pixels[i * newStride] = pixels[i * oldStride];			// R
		info.pixels[i * newStride + 1] = pixels[i * oldStride + 1];	// G
		info.pixels[i * newStride + 2] = pixels[i * oldStride + 2];	// B
		info.pixels[i * newStride + 3] = 0xFF;						// A (always 1)
	}

	info.stride = newStride;
}

Texture TextureHandler::LoadTexture(std::string filePath)
{
	Texture result = {};

	// Load image pixels with stb_image
	UINT8* pixels = stbi_load(filePath.c_str(), &result.width, &result.height, &result.stride, STBI_default);
	if (!pixels)
	{
		Debug::Error("Failed to load image", __FILENAME__, __LINE__);
	}

	FormatTexture(result, pixels);
	stbi_image_free(pixels);
	return result;
}

TextureHandler* TextureHandler::GetInstance()
{
	if (m_instance.get() == nullptr) 
		m_instance.reset(new TextureHandler);
	return m_instance.get();
}

void TextureHandler::Uninitialize()
{

}

