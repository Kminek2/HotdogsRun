#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Device.h"
#include "Buffer.h"
#include <memory>


class Descriptior;

template <typename T>
class UniformBuffer
{
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

    VkBufferUsageFlags bufferUsage;
    VkDeviceSize bufferSize;
public:
	UniformBuffer(const uint16_t& numberOfFrames, VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	~UniformBuffer();
	void UpdateBuffer(uint16_t currentFrame, const T& data);
    void UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize);
    VkDeviceSize getSize() { return bufferSize; };

	friend Descriptior;
};

template<typename T>
UniformBuffer<T>::UniformBuffer(const uint16_t& numberOfFrames, VkBufferUsageFlags bufferUsage)
{
    bufferSize = sizeof(T);

    uniformBuffers.resize(numberOfFrames);
    uniformBuffersMemory.resize(numberOfFrames);
    uniformBuffersMapped.resize(numberOfFrames);

    for (size_t i = 0; i < numberOfFrames; i++) {
        Buffer<T>::CreateBuffer(bufferSize, bufferUsage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(Device::getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

template<typename T>
UniformBuffer<T>::~UniformBuffer()
{
    for (size_t i = 0; i < uniformBuffers.size(); i++) {
        vkDestroyBuffer(Device::getDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(Device::getDevice(), uniformBuffersMemory[i], nullptr);
    }
}

template<typename T>
void UniformBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data)
{
    bufferSize = sizeof(data);
    memcpy(uniformBuffersMapped[currentFrame], &data, bufferSize);
}

template<typename T>
void UniformBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize)
{
    bufferSize = dataSize;
    memcpy(uniformBuffersMapped[currentFrame], &data, bufferSize);
}