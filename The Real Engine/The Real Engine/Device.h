#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#define MAX_NUMBER_OF_OBJ 1000

class Device
{
private:
	static VkPhysicalDevice physicalDevice;
	static VkDevice device;

	static std::vector<const char*> deviceExtensions;

	void PickPhysicalDevice();
	static bool isDeviceSuitable(VkPhysicalDevice device);
	static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	void CreateLogicalDevice();
public:
	Device(std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
	~Device();

	static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	static VkDevice& getDevice() { return device; }
	static VkPhysicalDevice& getPhysicalDevice() { return physicalDevice; }

	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

