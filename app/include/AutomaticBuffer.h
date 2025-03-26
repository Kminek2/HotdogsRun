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
    memcpy(this->uniformBuffersMapped[currentFrame], &data, this->bufferSize);
    uniform->UpdateDescriptorSets(this->uniformBuffers, binding, this->bufferSize);
}

template<typename T>
void AutomaticBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize)
{
    if (dataSize == 0)
        return;
    else if (dataSize != this->bufferSize)
        this->RecreateBuffer(dataSize);
    this->bufferSize = dataSize;
    memcpy(this->uniformBuffersMapped[currentFrame], &data, this->bufferSize);
    uniform->UpdateDescriptorSets(this->uniformBuffers, binding, this->bufferSize);
}