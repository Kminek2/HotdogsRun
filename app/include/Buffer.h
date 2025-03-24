#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Device.h"
#include "Commands.h"
#include <stdexcept>

#include <cstring>
#include <map>

template <typename T>
class UniformBuffer;

template <typename T>
class Buffer
{
private:
    bool createdBuffer = false;
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory;
    uint64_t actBufferSize = 0;

	std::vector<T> data;
    //std::vector<T> gpuData;

    //std::map<uint32_t, bool> sendThis;
    //bool lastData = true;

	VkBufferUsageFlags bufferUsage;
	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void* dataPtr;

    VkDeviceSize stagingBufferSize = 0;

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory;
public:
	Buffer(const VkBufferUsageFlags& bufferUsage);
	~Buffer();
	void AddToBuffer(T dataToStore);
	void AddToBuffer(std::vector<T> dataToStore);

	void SendBufferToMemory();

	uint32_t getSize() { return data.size(); }
    std::vector<T> getData() { return data; }
    VkBuffer& getBuffer() { return buffer; }

	void ClearBuffer();

    template <typename t>
    friend class UniformBuffer;
    friend class Texture;
    friend class CubeMap;
};

template<typename T>
Buffer<T>::~Buffer()
{
    if (actBufferSize != 0)
        vkUnmapMemory(Device::getDevice(), stagingBufferMemory);
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

    /*if ((gpuData.size() < data.size()) && !lastData) {
        sendThis.emplace(data.size() - 1, true);
        lastData = !lastData;
    }
    else if ((data[data.size() - 1] != gpuData[data.size() - 1]) && !lastData) {
        sendThis.emplace(data.size() - 1, true);
        lastData = !lastData;
    }
    else if(lastData){
        sendThis.emplace(data.size() - 1, false);
        lastData = !lastData;
    }*/

}

template<typename T>
void Buffer<T>::AddToBuffer(std::vector<T> dataToStore)
{
    data.insert(data.end(), dataToStore.begin(), dataToStore.end());
    /*for (int i = 0; i < dataToStore.size(); i++)
    {
        data.push_back(dataToStore[i]);

        if ((gpuData.size() < data.size()) && !lastData) {
            sendThis.emplace(data.size() - 1, true);
            lastData = !lastData;
        }
        else if ((data[data.size() - 1] != gpuData[data.size() - 1]) && !lastData) {
            sendThis.emplace(data.size() - 1, true);
            lastData = !lastData;
        }
        else if (lastData) {
            sendThis.emplace(data.size() - 1, false);
            lastData = !lastData;
        }
    }*/
}



template<typename T>
void Buffer<T>::SendBufferToMemory()
{
    // add mutex to guard this function
    VkDeviceSize bufferSize = sizeof(data[0]) * data.size();
    if (bufferSize == 0)
        return;

    if (bufferSize > actBufferSize)
        if (actBufferSize != 0)
            vkUnmapMemory(Device::getDevice(), stagingBufferMemory);


    if (stagingBufferSize < bufferSize) {
        if (stagingBuffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(Device::getDevice(), stagingBuffer, nullptr);
            vkFreeMemory(Device::getDevice(), stagingBufferMemory, nullptr);
        }
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        stagingBufferSize = bufferSize;
    }

    //sendThis.emplace(data.size(), !lastData);

    //VkDeviceSize pos = 0;
    //for (auto& chang : sendThis) {
        //void* data;
        //vkMapMemory(Device::getDevice(), stagingBufferMemory, pos, static_cast<VkDeviceSize>(chang.first), 0, &data);
        //memcpy(data, std::next(this->data.data(), pos), (size_t)((chang.first - pos) * sizeof(data[0])));
        //vkUnmapMemory(Device::getDevice(), stagingBufferMemory);
        //pos = chang.first;
    //}

    if (bufferSize > actBufferSize)
        vkMapMemory(Device::getDevice(), stagingBufferMemory, 0, bufferSize, 0, &dataPtr);

    memcpy(dataPtr, this->data.data(), (size_t)bufferSize);

    if (!createdBuffer || bufferSize > actBufferSize) {
        vkDeviceWaitIdle(Device::getDevice());
        if (buffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(Device::getDevice(), buffer, nullptr);
            vkFreeMemory(Device::getDevice(), bufferMemory, nullptr);
        }
        CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | bufferUsage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
        createdBuffer = true;
    }

    CopyBuffer(stagingBuffer, buffer, bufferSize);

    actBufferSize = bufferSize;

    //gpuData = this->data;
    //lastData = true;
    //sendThis.clear();
}

template<typename T>
void Buffer<T>::ClearBuffer()
{
    if(!data.empty())
        data.clear();
}