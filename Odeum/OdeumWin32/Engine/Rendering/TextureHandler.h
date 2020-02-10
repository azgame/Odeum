#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <memory>
#include <string>
#include <vector>

struct Texture
{

};

class TextureHandler
{
public:
	static TextureHandler* GetInstance();
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;
private:
	TextureHandler();
	~TextureHandler();
	void Uninitialize();
	static std::unique_ptr<TextureHandler> m_instance;
	friend std::default_delete<TextureHandler>;
};

#endif