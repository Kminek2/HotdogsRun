#include "Uniform.h"
#include "Device.h"
#include "Application.h"

#include <stdexcept>

void Uniform::CreateDescriptorPool()
{
    std::vector < VkDescriptorPoolSize> poolSize(types.size());

    for (int i = 0; i < types.size(); i++) {
        poolSize[i].type = types[i];
        poolSize[i].descriptorCount = static_cast<uint32_t>(FRAMES_IN_FLIGHT);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
    poolInfo.pPoolSizes = poolSize.data();
    poolInfo.maxSets = static_cast<uint32_t>(FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(Device::getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Uniform::AllocateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(Device::getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
}

void Uniform::UpdateImageInDescriptorSets(const Texture& texture, const uint32_t& binding)
{
    VkDescriptorImageInfo imageInfo;

    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture.imageView;
    imageInfo.sampler = texture.sampler;


    for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
        VkWriteDescriptorSet descriptorWrite{};

        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(Device::getDevice(), 1, &descriptorWrite, 0, nullptr);
    }
}

void Uniform::AddUniforms(uint16_t amount, VkDescriptorType type, VkShaderStageFlags shaderStage, uint32_t descriptorSize)
{
    for (int i = 0; i < amount; i++)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = UBOs.size();
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.descriptorCount = descriptorSize;
        uboLayoutBinding.stageFlags = shaderStage;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        types.push_back(type);
        UBOs.push_back(uboLayoutBinding);
    }
}

std::vector<VkDescriptorSetLayout> Uniform::BindUniforms()
{
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(UBOs.size());
    layoutInfo.pBindings = UBOs.data();

    if (vkCreateDescriptorSetLayout(Device::getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    CreateDescriptorPool();
    AllocateDescriptorSets();

    return { descriptorSetLayout };
}

std::vector<VkDescriptorSetLayout> Uniform::GetUnfiorms() {
    return { descriptorSetLayout };
}

Uniform::~Uniform() {
    if (descriptorPool != nullptr) {
        vkDestroyDescriptorPool(Device::getDevice(), descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(Device::getDevice(), descriptorSetLayout, nullptr);

        descriptorPool = nullptr;
    }
}