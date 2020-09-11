#ifndef VKCORE_H
#define VKCORE_H

#include "VKIncludes.h"

namespace VKGraphics
{

	void Initialize();
	void Resize(uint32_t width_, uint32_t height_);
	void Shutdown();
	void Present();

	extern uint32_t m_displayWidth;
	extern uint32_t m_displayHeight;

	uint64_t GetFrameCount();
	float GetFrameTime();
	float GetFrameRate();

	extern VkDevice m_device;
	extern VkInstance m_instance;

	enum ResolutionOptions { k720p, k900p, k1080p, k1440p, k1800p, k2160p };

	extern bool s_enableVSync;
	extern ResolutionOptions m_targetResolution;
}

#endif