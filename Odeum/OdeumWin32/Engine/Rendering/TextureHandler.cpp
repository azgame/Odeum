#include "TextureHandler.h"

std::unique_ptr<TextureHandler> TextureHandler::m_instance = nullptr;

TextureHandler::TextureHandler()
{
}


TextureHandler::~TextureHandler()
{
	Uninitialize();
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