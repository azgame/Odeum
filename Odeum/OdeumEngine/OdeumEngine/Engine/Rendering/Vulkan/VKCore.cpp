#include "VkCore.h"

#include "../../Core/OdeumEngine.h"

using namespace Graphics;

namespace VKGraphics
{
	uint32_t m_nativeWidth = 0;
	uint32_t m_nativeHeight = 0;

	// DisplayBuffer m_preDisplayBuffer

	VkDevice m_device;
	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_debugMessenger;

	void SetNativeResolution()
	{
		uint32_t nativeWidth, nativeHeight;

		switch (ResolutionOptions((int)s_targetResolution))
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

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	// Core
	void CreateInstance();
	void SetupDebugMessenger();
	void PickPhysicalDevice();

	// Utilities
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
}

void VKGraphics::Initialize()
{
	ASSERT(&sm_swapChain == nullptr, "VkGraphics has already been initialized");

	s_displayWidth = OdeumEngine::Get().GetWindow().GetWidth();
	s_displayHeight = OdeumEngine::Get().GetWindow().GetHeight();

	CreateInstance();
	// Message callbacks for errors -- do later
}

void VKGraphics::CreateInstance()
{
	ASSERT(enableValidationLayers && !CheckValidationLayerSupport(), "Validation layers request, but not available!");

	VkApplicationInfo appInfo{};
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

	auto glfwExtensions = GetRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}
	
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

void VKGraphics::SetupDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("Failed to set up debug messenger");
}

void VKGraphics::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support");


}

void VKGraphics::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

bool VKGraphics::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}

std::vector<const char*> VKGraphics::GetRequiredExtensions()
{
	std::vector<const char*> extensions;

	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(WIN32_LEAN_AND_MEAN)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

	if (enableValidationLayers)
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VKGraphics::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		Debug::Error("Validation layer: " + std::string(pCallbackData->pMessage), __FILENAME__, __LINE__);

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		Debug::Warning("Validation layer: " + std::string(pCallbackData->pMessage), __FILENAME__, __LINE__);

	return VK_FALSE;
}

VkResult VKGraphics::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void VKGraphics::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
		func(instance, debugMessenger, pAllocator);
}

void VKGraphics::Resize(uint32_t width_, uint32_t height_)
{
}

void VKGraphics::Shutdown()
{
	if (enableValidationLayers)
		DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);

	vkDestroyInstance(m_instance, nullptr);
}

void VKGraphics::Present()
{
}