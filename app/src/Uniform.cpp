#include "Uniform.h"
#include "Device.h"

#include <stdexcept>

VkDescriptorSetLayout Uniform::descriptorSetLayout;

void Uniform::AddUniforms(uint16_t amount, VkDescriptorType type, VkShaderStageFlags shaderStage)
{
    for (int i = 0; i < amount; i++)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = UBOs.size();
        uboLayoutBinding.descriptorType = type;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = shaderStage;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

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

    return { descriptorSetLayout };
}

Uniform::~Uniform() {
    vkDestroyDescriptorSetLayout(Device::getDevice(), descriptorSetLayout, nullptr);
}