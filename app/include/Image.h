#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdexcept>
#include <glm/glm.hpp>
#include <vector>

#include "Device.h"

struct Image
{
	VkImage image;
	VkImageView imageView;

    void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);

    static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int32_t offset, std::pair<int32_t, int32_t> imageOffset);

//    ~Image();
};

struct Images
{
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;

    uint32_t size = 0;

    void resize(uint32_t size);

    static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void CreateImages(std::vector<std::pair<int, int>> dimentions, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, std::vector<VkImage>& images, VkDeviceMemory& imageMemory, std::vector<uint32_t>& offsets);

//    ~Images();
};

struct Texture : Image {
    VkDeviceMemory textureMemory;
    VkSampler sampler;

    std::vector<std::pair<stbi_uc*, VkDeviceSize>> textures;
    std::pair<uint32_t, uint32_t> dimention = {0, 0};
    std::vector<std::pair<int, int>> dimensions;
    size_t allTextureSize = 0;

    void CreateSampler(VkFilter oversamplingFilter = VK_FILTER_NEAREST, VkFilter undersamplingFilter = VK_FILTER_LINEAR);

    glm::uvec2 GetImageSize(const char* texturePath);
    void AddTexture(const char* texturePath);

    void SendTexturesToMemory();

    ~Texture();
};