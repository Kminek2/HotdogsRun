#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<vector>

class Engine;

class ValidationLayers
{
private:
	VkDebugUtilsMessengerEXT debugMessenger;
	static std::vector<const char*> enabledValidationLayers;

	static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
public:
	ValidationLayers(std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" });
	~ValidationLayers();

	bool CheckValidationLayerSupport();
	
	static std::vector<const char*> getValidationLayers() { return enabledValidationLayers; };
	friend Engine;
};

