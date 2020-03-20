#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include "../pch.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

struct Texture
{
	std::vector<UINT8> pixels;
	int width = 0;
	int height = 0;
	int stride = 0;
	int offset = 0;
};

class TextureHandler
{
public:
	static TextureHandler* GetInstance();
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;

	void FormatTexture(Texture& info, UINT8* pixels);
	void CreateTexture(std::string textureName, std::string filePath);
	Texture GetTexture(std::string textureName);
private:
	TextureHandler();
	~TextureHandler();
	void Uninitialize();
	static std::unique_ptr<TextureHandler> m_instance;
	friend std::default_delete<TextureHandler>;

	std::map<std::string, Texture> m_textureMap;
};

#endif