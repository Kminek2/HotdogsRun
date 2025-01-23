#include "SwapChain.h"
#include "Engine.h"
#include "Queues.h"
#include "Device.h"
#include <algorithm>

VkSurfaceKHR SwapChain::surface;

SwapChain::SwapChain(uint16_t width, uint16_t height)
{
    swapChainImages = new Images();
    renderPass = nullptr;
    this->width = width;
    this->height = height;
    CreateSurface();
}

SwapChain::~SwapChain() {
    delete renderPass;

    CleanUp();

    delete swapChainImages;

    vkDestroySurfaceKHR(Engine::instance, surface, nullptr);
}

void SwapChain::CleanUp() {
    for (size_t i = 0; i < framebuffers.size(); i++) {
        delete framebuffers[i];
    }

    swapChainImages->resize(0);

    vkDestroySwapchainKHR(Device::getDevice(), swapChain, nullptr);
}

void SwapChain::ReCreate() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(Engine::window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(Engine::window, &width, &height);
        glfwWaitEvents();
    }

    this->width = width;
    this->height = height;

    vkDeviceWaitIdle(Device::getDevice());

    CleanUp();

    CreateSwapChain();
    CreateImageViews();
    renderPass->RecreateDepthResource();
    CreateFrameBuffers();
}

void SwapChain::CreateSurface()
{
    if (glfwCreateWindowSurface(Engine::instance, Engine::window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void SwapChain::CreateWholeSwapChain() {
    
    CreateSwapChain();
    CreateImageViews();
    renderPass = new RenderPass(this);
    CreateFrameBuffers();
}

void SwapChain::CreateFrameBuffers() {
    framebuffers.resize(swapChainImages->imageViews.size());

    for (int i = 0; i < swapChainImages->imageViews.size(); i++)
    {
        framebuffers[i] = new Framebuffer();
        framebuffers[i]->CreateFramebuffer(swapChainImages->imageViews[i], renderPass, this);
    }
}

void SwapChain::CreateSwapChain() {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(Device::getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 3; // + 3 <- (my prefared reserve count)
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT <- for transforming pixels from one image to the nex if we want eg. post-processing

    Queues::QueueFamilyIndices indices = Queues::FindQueueFamilies(Device::getPhysicalDevice());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(Device::getDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(Device::getDevice(), swapChain, &imageCount, nullptr);
    swapChainImages->resize(imageCount);
    vkGetSwapchainImagesKHR(Device::getDevice(), swapChain, &imageCount, swapChainImages->images.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;


}

VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(Engine::window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void SwapChain::CreateImageViews() {
    for (size_t i = 0; i < swapChainImages->imageViews.size(); i++) {
        swapChainImages->imageViews[i] = Images::CreateImageView(swapChainImages->images[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

SwapChain::SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkPhysicalDevice device) {
    SwapChain::SwapChainSupportDetails details;

    VkSurfaceKHR& surface = SwapChain::getSurface();

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}