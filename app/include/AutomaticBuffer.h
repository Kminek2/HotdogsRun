#pragma once
#include "UniformBuffer.h"
#include "Uniform.h"
template<typename T>
class AutomaticBuffer :
    public UniformBuffer<T>
{
private:
    Uniform* uniform;
    uint32_t binding;

public:
    AutomaticBuffer(const uint16_t& numberOfFrames, Uniform* unform, uint32_t binding, VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    void UpdateBuffer(uint16_t currentFrame, const T& data) override;
    void UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize) override;
};

template<typename T>
inline AutomaticBuffer<T>::AutomaticBuffer(const uint16_t& numberOfFrames, Uniform* unform, uint32_t binding, VkBufferUsageFlags bufferUsage) : UniformBuffer<T>(numberOfFrames, bufferUsage)
{
    this->uniform = uniform;
    this->binding = binding;
}

template<typename T>
void AutomaticBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data)
{
    memcpy(uniformBuffersMapped[currentFrame], &data, bufferSize);
    uniform->UpdateDescriptorSets(uniformBuffers, binding, bufferSize);
}

template<typename T>
void AutomaticBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize)
{
    if (dataSize == 0)
        return;
    else if (dataSize != bufferSize)
        RecreateBuffer(dataSize);
    bufferSize = dataSize;
    memcpy(uniformBuffersMapped[currentFrame], &data, bufferSize);
    uniform->UpdateDescriptorSets(uniformBuffers, binding, bufferSize);
}