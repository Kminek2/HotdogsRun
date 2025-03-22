#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stdexcept>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#include "Device.h"

class Uniform;

struct Image
{
	VkImage image = VK_NULL_HANDLE;
	VkImageView imageView;

    void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1);

    static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT, uint32_t arrayLeyers = 1, VkImageCreateFlags flags = 0);

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount = 1, VkImageAspectFlagBits aspecFlag = VK_IMAGE_ASPECT_COLOR_BIT);
    static void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount = 1, VkImageAspectFlagBits aspecFlag = VK_IMAGE_ASPECT_COLOR_BIT);

    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int32_t offset, std::pair<int32_t, int32_t> imageOffset, uint32_t layerCount = 1);
    static void copyImage(VkImage srcImage, VkImage dstImage, VkImageLayout srcLayout, VkImageLayout dstLayout, glm::uvec2 extent);
    
    static void CreateSampler(VkSampler& sampler, VkFilter oversamplingFilter, VkFilter undersamplingFilter);

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
    VkSampler sampler = VK_NULL_HANDLE;

    std::vector<std::pair<stbi_uc*, VkDeviceSize>> textures;
    std::pair<uint32_t, uint32_t> dimention = {0, 0};
    std::vector<std::pair<int, int>> dimensions;
    uint32_t alreadyLoaded = 0;
    size_t allTextureSize = 0;

    int32_t bufferOffset = 0;
    int32_t heightOffset = 0;

    Uniform* uniform = nullptr;
    uint32_t binding = 0;

    void SetBinding(Uniform* uniform, uint32_t binding);

    void CreateSampler(VkFilter oversamplingFilter = VK_FILTER_NEAREST, VkFilter undersamplingFilter = VK_FILTER_LINEAR);

    glm::uvec2 GetImageSize(const char* texturePath);
    void AddTexture(const char* texturePath);

    void SendTexturesToMemory();

    ~Texture();
};

struct CubeMap : Image {
    VkDeviceMemory textureMemory;
    VkSampler sampler;

    /// <summary>
    /// Creates cube map
    /// </summary>
    /// <param name="textures">X, -X, Y, -Y, Z, -Z</param>
    CubeMap(std::array<std::string, 6> textures);
};