#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Device.h"
#include "Commands.h"
#include <stdexcept>

#include <cstring>

template <typename T>
class UniformBuffer;

template <typename T>
class Buffer
{
private:
    bool createdBuffer = false;
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
    uint64_t actBufferSize = 0;

	std::vector<T> data;

	VkBufferUsageFlags bufferUsage;
	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);



    VkDeviceSize stagingBufferSize = 0;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
public:
	Buffer(const VkBufferUsageFlags& bufferUsage);
	~Buffer();
	void AddToBuffer(T dataToStore);
	void AddToBuffer(std::vector<T> dataToStore);

	void SendBufferToMemory();

	uint32_t getSize() { return data.size(); }
    VkBuffer& getBuffer() { return buffer; }

	void ClearBuffer();

    template <typename t>
    friend class UniformBuffer;
    friend class Texture;
};

template<typename T>
Buffer<T>::~Buffer()
{
	vkDestroyBuffer(Device::getDevice(), buffer, nullptr);
	vkFreeMemory(Device::getDevice(), bufferMemory, nullptr);
    vkDestroyBuffer(Device::getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(Device::getDevice(), stagingBufferMemory, nullptr);
}

template<typename T>
void Buffer<T>::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Device::getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Device::getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Device::FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Device::getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Device::getDevice(), buffer, bufferMemory, 0);
}

template<typename T>
void Buffer<T>::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = Commands::BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    Commands::EndSingleTimeCommands(commandBuffer);
}

template<typename T>
Buffer<T>::Buffer(const VkBufferUsageFlags& bufferUsage)
{
    this->bufferUsage = bufferUsage;
}

template<typename T>
void Buffer<T>::AddToBuffer(T dataToStore)
{
    data.push_back(dataToStore);
}

template<typename T>
void Buffer<T>::AddToBuffer(std::vector<T> dataToStore)
{
    data.insert(data.end(), dataToStore.begin(), dataToStore.end());
}



template<typename T>
void Buffer<T>::SendBufferToMemory()
{
    VkDeviceSize bufferSize = sizeof(data[0]) * data.size();
    if (stagingBufferSize < bufferSize) {
        vkDestroyBuffer(Device::getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(Device::getDevice(), stagingBufferMemory, nullptr);
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        stagingBufferSize = bufferSize;
    }

    void* data;
    vkMapMemory(Device::getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, this->data.data(), (size_t)bufferSize);
    vkUnmapMemory(Device::getDevice(), stagingBufferMemory);

    if (!createdBuffer || bufferSize > actBufferSize) {
        vkDeviceWaitIdle(Device::getDevice());
        vkDestroyBuffer(Device::getDevice(), buffer, nullptr);
        vkFreeMemory(Device::getDevice(), bufferMemory, nullptr);
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
        createdBuffer = true;
    }

    CopyBuffer(stagingBuffer, buffer, bufferSize);

    actBufferSize = bufferSize;
}

template<typename T>
void Buffer<T>::ClearBuffer()
{
    if(!data.empty())
        data.clear();
}