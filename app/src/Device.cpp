#include "Device.h"
#include "Engine.h"
#include "Queues.h"
#include "SwapChain.h"

#include <set>
#include <vector>

VkDevice Device::device;
VkPhysicalDevice Device::physicalDevice;
std::vector<const char*> Device::deviceExtensions;
VkSampleCountFlagBits Device::msaaSamples = VK_SAMPLE_COUNT_1_BIT;


Device::Device(std::vector<const char*> deviceExtensions)
{
    this->deviceExtensions = deviceExtensions;
	PickPhysicalDevice();
    CreateLogicalDevice();
}

Device::~Device() {
    vkDestroyDevice(device, nullptr);
}

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


void Device::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(Engine::instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
#ifdef _WIN32
        MessageBox(
            NULL,
            L"Update your drivers and try again.",
            L"No compatable device found",
            MB_ICONERROR | MB_OK | MB_DEFBUTTON1
        );
#endif // _WIN32

        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);  //zmieni� na r�czny wyb�r lub sutability check przy s�abej wydajno�ci
    vkEnumeratePhysicalDevices(Engine::instance, &deviceCount, devices.data());

    std::pair<uint32_t, VkPhysicalDevice> scores {0, nullptr};

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            if (scores.first < GetScore(device))
                scores = { GetScore(device), device };
        }
    }

    physicalDevice = scores.second;

    if (physicalDevice == VK_NULL_HANDLE) {
#ifdef _WIN32
        MessageBox(
            NULL,
            L"Update your drivers and try again.",
            L"No compatable device found",
            MB_ICONERROR | MB_OK | MB_DEFBUTTON1
        );
#endif // _WIN32

        throw std::runtime_error("failed to find a suitable GPU!");
    }

    msaaSamples = GetMaxUsableSampleCount(physicalDevice);
}

uint32_t Device::GetScore(VkPhysicalDevice device) {
    uint32_t score = 1;

    VkPhysicalDeviceProperties deviceProperites;
    vkGetPhysicalDeviceProperties(device, &deviceProperites);

    if (deviceProperites.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += GetMaxUsableSampleCount(device);

    return score;
}

bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    Queues::QueueFamilyIndices indices = Queues::FindQueueFamilies(device);

    bool extensionsSupported = CheckDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void Device::CreateLogicalDevice() {
    Queues::QueueFamilyIndices indices = Queues::FindQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    ValidationLayers* validationLayers = Engine::getValidationLayers();

    if (validationLayers != nullptr) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(Engine::enValidationLayers.size());
        createInfo.ppEnabledLayerNames = Engine::enValidationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &(Queues::graphicsQueue));
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &(Queues::presentQueue));
}

VkFormat Device::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

uint32_t Device::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

VkSampleCountFlagBits Device::GetMaxUsableSampleCount(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}


VkSampleCountFlagBits Device::GetSampleCount()
{
    return msaaSamples;
}