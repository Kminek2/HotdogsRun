/*#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include "Device.h"
#include "UniformBuffer.h"
#include "CameraStructures.h"
#include "Uniform.h"
#include "LightBufferStruct.h"
#include <array>
//IDK, I really don't want to think anymore. It's to hard :(
// I should merge Uniform and this class but I no longer think
//ITs hardcoded for now

class Commands;

class Descriptior
{
private:
    const unsigned int MAX_FRAMES_IN_FLIGHT;

    VkDescriptorPool descriptorPool;
    static std::vector<VkDescriptorSet> descriptorSets;
public:
    void CreateDescriptorPool() {
        std::array<VkDescriptorPoolSize, 4> poolSize{};
        poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        poolSize[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        poolSize[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSize[3].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
        poolInfo.pPoolSizes = poolSize.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(Device::getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void CreateDescriptorSets(const UniformBuffer<UniformCameraBuffer>& uniformBuffer, const Texture& texture, const UniformBuffer<PointLightBuffer>& uniformPointLightStruct, const UniformBuffer<SpotLightBuffer>& uniformSpotLightStruct) {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, Uniform::descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(Device::getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        VkDescriptorImageInfo imageInfo;

        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.imageView;
        imageInfo.sampler = texture.sampler;


        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            std::array<VkDescriptorBufferInfo, 3> bufferInfo{};
            bufferInfo[0].buffer = uniformBuffer.uniformBuffers[i];
            bufferInfo[0].offset = 0;
            bufferInfo[0].range = uniformBuffer.bufferSize;

            bufferInfo[1].buffer = uniformPointLightStruct.uniformBuffers[i];
            bufferInfo[1].offset = 0;
            bufferInfo[1].range = uniformPointLightStruct.bufferSize;

            bufferInfo[2].buffer = uniformSpotLightStruct.uniformBuffers[i];
            bufferInfo[2].offset = 0;
            bufferInfo[2].range = uniformSpotLightStruct.bufferSize;

            std::array<VkWriteDescriptorSet, 4> descriptorWrite{};
            descriptorWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[0].dstSet = descriptorSets[i];
            descriptorWrite[0].dstBinding = 0;
            descriptorWrite[0].dstArrayElement = 0;
            descriptorWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite[0].descriptorCount = 1;
            descriptorWrite[0].pBufferInfo = &bufferInfo[0];
            descriptorWrite[0].pImageInfo = nullptr; // Optional
            descriptorWrite[0].pTexelBufferView = nullptr; // Optional

            descriptorWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[1].dstSet = descriptorSets[i];
            descriptorWrite[1].dstBinding = 1;
            descriptorWrite[1].dstArrayElement = 0;
            descriptorWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite[1].descriptorCount = 1;
            descriptorWrite[1].pImageInfo = &imageInfo;

            descriptorWrite[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[2].dstSet = descriptorSets[i];
            descriptorWrite[2].dstBinding = 2;
            descriptorWrite[2].dstArrayElement = 0;
            descriptorWrite[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrite[2].descriptorCount = 1;
            descriptorWrite[2].pBufferInfo = &bufferInfo[1];
            descriptorWrite[2].pImageInfo = nullptr; // Optional
            descriptorWrite[2].pTexelBufferView = nullptr; // Optional

            descriptorWrite[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[3].dstSet = descriptorSets[i];
            descriptorWrite[3].dstBinding = 3;
            descriptorWrite[3].dstArrayElement = 0;
            descriptorWrite[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrite[3].descriptorCount = 1;
            descriptorWrite[3].pBufferInfo = &bufferInfo[2];
            descriptorWrite[3].pImageInfo = nullptr; // Optional
            descriptorWrite[3].pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(Device::getDevice(), static_cast<uint32_t>(descriptorWrite.size()), descriptorWrite.data(), 0, nullptr);
        }
    }

    Descriptior(unsigned int MAX_FRAMES_IN_FLIGHT, const UniformBuffer<UniformCameraBuffer>& uniformBuffer, const Texture& texture, const UniformBuffer<PointLightBuffer>& uniformPointLightStruct, const UniformBuffer<SpotLightBuffer>& uniformSpotLightStruct) : MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT)
    {
        CreateDescriptorPool();
        CreateDescriptorSets(uniformBuffer, texture, uniformPointLightStruct, uniformSpotLightStruct);
    };

    template <typename T>
    void UpdateDescriptor(const UniformBuffer<T> &buffer, uint32_t binding, uint32_t size)
    {
        vkDeviceWaitIdle(Device::getDevice());
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = buffer.uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = size; // sizeof(Light) * numLights

            VkWriteDescriptorSet descriptorWrite = {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = binding;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(Device::getDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }

    ~Descriptior() {
        vkDestroyDescriptorPool(Device::getDevice(), descriptorPool, nullptr);
    }

    friend Commands;
};

*/