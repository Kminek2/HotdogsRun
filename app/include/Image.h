#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdexcept>

#include <vector>

#include "Device.h"

struct Image
{
	VkImage image;
	VkImageView imageView;

    void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);

    static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

struct Images
{
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;

    uint32_t size = 0;

    void resize(uint32_t size);

    static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void CreateImages(std::vector<std::pair<int, int>> dimentions, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, std::vector<VkImage>& images, VkDeviceMemory& imageMemory);
};

struct Textures : Images {
    VkDeviceMemory textureMemory;
    VkSampler sampler;

    std::vector<stbi_uc*> textures;
    std::vector<std::pair<int, int>> dimentions;
    uint32_t allTextureSize = 0;

    void CreateSampler(VkFilter oversamplingFilter = VK_FILTER_NEAREST, VkFilter undersamplingFilter = VK_FILTER_LINEAR);

    void AddTexture(const char* texturePath);

    void SendTexturesToMemory();
};