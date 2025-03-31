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
    AutomaticBuffer(const uint16_t& numberOfFrames, VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    AutomaticBuffer(const uint16_t& numberOfFrames, Uniform* unform, uint32_t binding, VkBufferUsageFlags bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    void SetBinding(Uniform* unform, uint32_t binding);
    void UpdateBuffer(uint16_t currentFrame, const T& data) override;
    void UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize) override;
};

template<typename T>
inline AutomaticBuffer<T>::AutomaticBuffer(const uint16_t& numberOfFrames, VkBufferUsageFlags bufferUsage) : UniformBuffer<T>(numberOfFrames, bufferUsage)
{
    this->uniform = nullptr;
    this->binding = 0;
}

template<typename T>
inline AutomaticBuffer<T>::AutomaticBuffer(const uint16_t& numberOfFrames, Uniform* unform, uint32_t binding, VkBufferUsageFlags bufferUsage) : UniformBuffer<T>(numberOfFrames, bufferUsage)
{
    this->uniform = uniform;
    this->binding = binding;
}

template<typename T>
inline void AutomaticBuffer<T>::SetBinding(Uniform* unform, uint32_t binding)
{
    if (uniform != unform)
        throw std::exception("This buffer already has uniform");

    this->uniform = uniform;
    this->binding = binding;
}

template<typename T>
void AutomaticBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data)
{
    if (uniform == nullptr)
        throw std::exception("This ain't right");

    memcpy(this->uniformBuffersMapped[currentFrame], &data, this->bufferSize);
    uniform->UpdateDescriptorSets(this->uniformBuffers, binding, this->bufferSize);
}

template<typename T>
void AutomaticBuffer<T>::UpdateBuffer(uint16_t currentFrame, const T& data, size_t dataSize)
{
    if (uniform == nullptr)
        throw std::exception("This ain't right");

    if (dataSize == 0)
        return;
    else if (dataSize != this->bufferSize)
        this->RecreateBuffer(dataSize);
    this->bufferSize = dataSize;
    memcpy(this->uniformBuffersMapped[currentFrame], &data, this->bufferSize);
    uniform->UpdateDescriptorSets(this->uniformBuffers, binding, this->bufferSize);
}