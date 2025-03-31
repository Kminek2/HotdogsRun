#include "Image.h"
#include "Commands.h"
#include "Buffer.h"
#include "Uniform.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void Image::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType, uint32_t layerCount) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = viewType;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = layerCount;

    if (vkCreateImageView(Device::getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void Image::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkSampleCountFlagBits sampleCount, uint32_t arrayLeyers, VkImageCreateFlags flags) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = arrayLeyers;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = sampleCount;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = flags;

    if (vkCreateImage(Device::getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Device::getDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Device::FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Device::getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Device::getDevice(), image, imageMemory, 0);
}

void Image::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount, VkImageAspectFlagBits aspecFlag) {
    VkCommandBuffer commandBuffer = Commands::BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspecFlag;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = layerCount;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }


    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    Commands::EndSingleTimeCommands(commandBuffer);
}

void Image::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount, VkImageAspectFlagBits aspecFlag)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = aspecFlag;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = layerCount;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }


    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

void Image::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, int32_t offset, std::pair<int32_t, int32_t> imageOffset, uint32_t layerCount) {
    VkCommandBuffer commandBuffer = Commands::BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = offset;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = layerCount;

    region.imageOffset = { imageOffset.first, imageOffset.second, 0};
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    Commands::EndSingleTimeCommands(commandBuffer);
}

void Image::copyImage(VkImage srcImage, VkImage dstImage, VkImageLayout srcLayout, VkImageLayout dstLayout, glm::uvec2 extent)
{
    VkCommandBuffer commandBuffer = Commands::BeginSingleTimeCommands();

    VkImageSubresourceLayers srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    VkImageSubresourceLayers dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };

    VkImageCopy region{};
    region.srcSubresource = srcSubresource;
    region.srcOffset = { 0, 0, 0 };
    region.dstSubresource = dstSubresource;
    region.dstOffset = { 0, 0, 0 };
    region.extent = { extent.x, extent.y, 1 };

    vkCmdCopyImage(
        commandBuffer,
        srcImage,
        srcLayout,
        dstImage,
        dstLayout,
        1,

        &region
    );

    Commands::EndSingleTimeCommands(commandBuffer);
}

void Image::CreateSampler(VkSampler& sampler, VkFilter oversamplingFilter, VkFilter undersamplingFilter)
{
    //Change maxAnisotropy to some value: min(value, properties.limit)
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(Device::getPhysicalDevice(), &properties);
    //
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = oversamplingFilter;
    samplerInfo.minFilter = undersamplingFilter;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(Device::getDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }

}

//Image::~Image()
//{
//    vkDestroyImageView(Device::getDevice(), imageView, nullptr);
//    vkDestroyImage(Device::getDevice(), image, nullptr);
//}

//Images

void Images::resize(uint32_t size)
{
    for (int i = 0; i < imageViews.size(); i++) {
        vkDestroyImageView(Device::getDevice(), imageViews[i], nullptr);
    }

    images.resize(size);
    imageViews.resize(size);

    this->size = size;
}

VkImageView Images::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
    VkImageView imageView;

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(Device::getDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

void Images::CreateImages(std::vector<std::pair<int, int>> dimentions, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, std::vector<VkImage>& images, VkDeviceMemory& imageMemory, std::vector<uint32_t>& offsets)
{
    VkDeviceSize totalSize = 0;
    VkDeviceSize alignment = 0;
    std::vector<VkMemoryRequirements> memRequirements(images.size());

    for (size_t i = 0; i < images.size(); i++) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = dimentions[i].first;
        imageInfo.extent.height = dimentions[i].second;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(Device::getDevice(), &imageInfo, nullptr, &images[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }


        vkGetImageMemoryRequirements(Device::getDevice(), images[i], &memRequirements[i]);

        alignment = std::max(alignment, memRequirements[i].alignment);
        totalSize = (totalSize + alignment - 1) & ~(alignment - 1);
        totalSize += memRequirements[i].size;
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = totalSize;
    allocInfo.memoryTypeIndex = Device::FindMemoryType(memRequirements[0].memoryTypeBits, properties);

    if (vkAllocateMemory(Device::getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate device memory!");
    }

    VkDeviceSize offset = 0;
    uint32_t stagingOffset = 0;
    for (size_t i = 0; i < images.size(); i++) {
        offset = (offset + memRequirements[i].alignment - 1) & ~(memRequirements[i].alignment - 1); // Align
        offsets.push_back(stagingOffset);
        vkBindImageMemory(Device::getDevice(), images[i], imageMemory, offset);
        offset += memRequirements[i].size;
        stagingOffset += dimentions[i].first * dimentions[i].second * 4;
    }
}

//Images::~Images() {
//    for (int i = 0; i < imageViews.size(); i++) {
//        vkDestroyImage(Device::getDevice(), images[i], nullptr);
//    }
//
//    resize(0);
//}

//Textures

void Texture::SetBinding(Uniform* uniform, uint32_t binding)
{
    this->uniform = uniform;
    this->binding = binding;
}

void Texture::CreateSampler(VkFilter oversamplingFilter, VkFilter undersamplingFilter) {
    //Change maxAnisotropy to some value: min(value, properties.limit)
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(Device::getPhysicalDevice(), &properties);
    //
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = oversamplingFilter;
    samplerInfo.minFilter = undersamplingFilter;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(Device::getDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

glm::uvec2 Texture::GetImageSize(const char* texturePath) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    stbi_image_free(pixels);
    return glm::uvec2(texWidth, texHeight);
}

void Texture::AddTexture(const char* texturePath) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texturePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(Device::getPhysicalDevice(), &deviceProperties);
    VkDeviceSize reqAlignment = deviceProperties.limits.nonCoherentAtomSize;

    VkDeviceSize totalSize = (imageSize + reqAlignment - 1) & ~(reqAlignment - 1);

    
    allTextureSize += totalSize;

    textures.push_back({ pixels, allTextureSize - totalSize});
    dimention = std::pair<uint32_t, uint32_t>(std::max(dimention.first, static_cast<uint32_t>(texWidth)), dimention.second + texHeight );
    dimensions.push_back({ texWidth, texHeight });
}

void Texture::SendTexturesToMemory() {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    bool isImageNull = image == VK_NULL_HANDLE;

    Buffer<bool>::CreateBuffer(allTextureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    for (int i = 0; i < textures.size(); i++) {
        vkMapMemory(Device::getDevice(), stagingBufferMemory, textures[i].second, dimensions[i].first * dimensions[i].second * 4, 0, &data);
        memcpy(data, textures[i].first, static_cast<size_t>(dimensions[i].first * dimensions[i].second * 4));
        vkUnmapMemory(Device::getDevice(), stagingBufferMemory);
    }
    for (auto& pixels : textures)
        stbi_image_free(pixels.first);

    VkDeviceMemory newImageMemory;
    VkImage newImage;
    CreateImage(dimention.first, dimention.second, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, newImage, newImageMemory);

    Image::transitionImageLayout(newImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    if (!isImageNull) {
        Image::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        copyImage(image, newImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { dimention.first, heightOffset });
        
        vkDestroyImageView(Device::getDevice(), imageView, nullptr);
        vkDestroyImage(Device::getDevice(), image, nullptr);
        vkFreeMemory(Device::getDevice(), textureMemory, nullptr);
    }

    image = newImage;
    textureMemory = newImageMemory;

    for (int i = alreadyLoaded; i < dimensions.size(); i++) {
        Image::copyBufferToImage(stagingBuffer, image, dimensions[i].first, dimensions[i].second, static_cast<int32_t>(textures[i - alreadyLoaded].second), {0, heightOffset});
        bufferOffset += textures[i - alreadyLoaded].second; // dimensions[i].first * dimensions[i].second * 4;
        heightOffset += dimensions[i].second;
    }

    alreadyLoaded = dimensions.size();
    Image::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    textures.clear();

    vkDestroyBuffer(Device::getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Device::getDevice(), stagingBufferMemory, nullptr);

    imageView = Images::CreateImageView(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    if (uniform != nullptr)
        uniform->UpdateImageInDescriptorSets(*this, binding);
}

Texture::~Texture()
{
    vkDestroySampler(Device::getDevice(), sampler, nullptr);
    vkDestroyImageView(Device::getDevice(), imageView, nullptr);
    vkDestroyImage(Device::getDevice(), image, nullptr);
    vkFreeMemory(Device::getDevice(), textureMemory, nullptr);

}

CubeMap::CubeMap(std::array<std::string, 6> textures)
{
    int texWidth{ 0 };
    int texHeight{ 0 };
    int texChannels{ 0 };

    std::array<stbi_uc*, 6> pixels;
    for(int i = 0; i < 6; i++)
        pixels[i] = stbi_load(textures[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize layerSize = texWidth * texHeight * 4;
    VkDeviceSize imageSize = layerSize * 6;

    if (!pixels[0]) {
        throw std::runtime_error("failed to load cubeMap images!");
    }


    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Buffer<bool>::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    for (int i = 0; i < pixels.size(); i++) {
        vkMapMemory(Device::getDevice(), stagingBufferMemory, layerSize * i, layerSize, 0, &data);
        memcpy(data, pixels[i], static_cast<size_t>(layerSize));
        vkUnmapMemory(Device::getDevice(), stagingBufferMemory);
    }

    for (auto& pixel : pixels)
        stbi_image_free(pixel);

    CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, textureMemory, VK_SAMPLE_COUNT_1_BIT, 6, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT);

    int32_t bufferOffset = 0;
    int32_t heightOffset = 0;

    Image::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6);
    Image::copyBufferToImage(stagingBuffer, image, texWidth, texHeight, 0, { 0, 0 }, 6);
    Image::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6);

    vkDestroyBuffer(Device::getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Device::getDevice(), stagingBufferMemory, nullptr);

    CreateImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE, 6);

    CreateSampler(sampler, VK_FILTER_LINEAR, VK_FILTER_LINEAR);
}
