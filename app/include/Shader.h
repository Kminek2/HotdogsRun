#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

class Shader
{
private:
	std::vector<char> shaderCode;
	VkShaderModule shaderModule;
	VkPipelineShaderStageCreateInfo shaderStageInfo;

	static VkShaderModule CreateShaderModule(const std::vector<char>& code);
	static VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage, VkSpecializationInfo& specializationInfo);
	static VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(VkShaderModule shaderModule, VkShaderStageFlagBits shaderStage);
public:
	Shader() {};
	Shader(const std::string filePath, VkShaderStageFlagBits shaderStage);
	Shader(const std::string filePath, VkShaderStageFlagBits shaderStage, VkSpecializationInfo& specializationInfo);
	~Shader();

	VkPipelineShaderStageCreateInfo getShaderStageInfo() const { return shaderStageInfo; }
};

