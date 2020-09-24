#ifndef VKCORE_H
#define VKCORE_H

#include "VkIncludes.h"
#include "../Common/GraphicsCore.h"

namespace VKGraphics
{

	void Initialize();
	void Resize(uint32_t width_, uint32_t height_);
	void Shutdown();
	void Present();

	extern VkDevice m_device;
	extern VkInstance m_instance;
}

#endif