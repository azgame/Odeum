#include "VKCore.h"

#define SWAP_CHAIN_BUFFER_COUNT 3

namespace VKGraphics
{

	float s_frameTime = 0.0f;
	uint64_t s_frameIndex = 0;
	int64_t s_frameStartTick = 0;

	uint32_t m_nativeWidth = 0;
	uint32_t m_nativeHeight = 0;
	uint32_t m_displayWidth = 0;
	uint32_t m_displayHeight = 0;

	bool s_enableVSync = false;

	UINT m_currentBuffer = 0;

	ResolutionOptions m_targetResolution = k1440p;

	// DisplayBuffer m_preDisplayBuffer

	VkDevice m_device;
	VkInstance m_instance;

	void SetNativeResolution()
	{
		uint32_t nativeWidth, nativeHeight;

		switch (ResolutionOptions((int)m_targetResolution))
		{
		default:
		case k720p:
			nativeWidth = 1280;
			nativeHeight = 720;
			break;
		case k900p:
			nativeWidth = 1600;
			nativeHeight = 900;
			break;
		case k1080p:
			nativeWidth = 1920;
			nativeHeight = 1080;
			break;
		case k1440p:
			nativeWidth = 2560;
			nativeHeight = 1440;
			break;
		case k1800p:
			nativeWidth = 3200;
			nativeHeight = 1800;
			break;
		case k2160p:
			nativeWidth = 3840;
			nativeHeight = 2160;
			break;
		}

		if (m_nativeWidth == nativeWidth && m_nativeHeight == nativeHeight)
			return;

		m_nativeWidth = nativeWidth;
		m_nativeHeight = nativeHeight;

		// IdleGpu();
		// InitializeScreenBuffers(nativeWidth, nativeHeight);
	}

	// DisplayBuffer m_displayPlane[SWAP_CHAIN_BUFFER_COUNT];

	VkSwapchainKHR sm_swapChain;
}

void VKGraphics::Initialize()
{
	ASSERT(&sm_swapChain == nullptr, "VkGraphics has already been initialized");

	VkApplicationInfo appInfo {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Odeum";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Odeum Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
	{
		Debug::FatalError("Could not create VK instance", __FILENAME__, __LINE__);
		throw std::runtime_error("Could not create VK instance");
	}
		
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());


}

void VKGraphics::Resize(uint32_t width_, uint32_t height_)
{
}

void VKGraphics::Shutdown()
{
	vkDestroyInstance(m_instance, nullptr);
}

void VKGraphics::Present()
{
}

uint64_t VKGraphics::GetFrameCount()
{
	return uint64_t();
}

float VKGraphics::GetFrameTime()
{
	return 0.0f;
}

float VKGraphics::GetFrameRate()
{
	return 0.0f;
}