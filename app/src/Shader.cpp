#include "Shader.h"
#include "Device.h"
#include <stdexcept>
#include "FileReader.h"
#include <iostream>

Shader::Shader(const std::string filePath, VkShaderStageFlagBits shaderStage, VkSpecializationInfo& specializationInfo)
{
    shaderCode = FileReader::Read(filePath);
    shaderModule = CreateShaderModule(shaderCode);
    shaderStageInfo = CreatePipelineShaderStageCreateInfo(shaderModule, shaderStage, specializationInfo);
}

Shader::Shader(const std::string filePath, VkShaderStageFlagBits shaderStage)
{
    std::cout << "Reading from path " << filePath << '\n';
    shaderCode = FileReader::Read(filePath);
    std::cout << "Read file " << filePath << '\n';
    shaderModule = CreateShaderModule(shaderCode);
    shaderStageInfo = CreatePipelineShaderStageCreateInfo(shaderModule, shaderStage);
}

Shader::~Shader()
{
    vkDestroyShaderModule(Device::getDevice(), shaderModule, nullptr);
}

VkShaderModule Shader::CreateShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(Device::getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

VkPipelineShaderStageCreateInfo Shader::CreatePipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage, VkSpecializationInfo& specializationInfo) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderStage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    shaderStageInfo.pSpecializationInfo = &specializationInfo; // Attach specialization info

    return shaderStageInfo;
}

VkPipelineShaderStageCreateInfo Shader::CreatePipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = shaderStage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}