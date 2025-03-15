#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Image.h"
#include "UniformBuffer.h"
#include "Application.h"

#include <array>

class Uniform
{
private:
	std::vector<VkDescriptorSetLayoutBinding> UBOs;
	std::vector<VkDescriptorType> types;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;

	void CreateDescriptorPool();
	void AllocateDescriptorSets();
public:
	void AddUniforms(uint16_t amount = 1, VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VkShaderStageFlags shaderStage = VK_SHADER_STAGE_VERTEX_BIT, uint32_t descriptorSize = 1);
	std::vector<VkDescriptorSetLayout> BindUniforms();
	std::vector<VkDescriptorSetLayout> GetUnfiorms();
	VkDescriptorSet& GetDescriptorSet(unsigned int frame) { return descriptorSets.at(frame); };
	~Uniform();

	void UpdateDescriptorSets(const std::vector<VkBuffer>& buffer, uint32_t binding, VkDeviceSize size);
	void UpdateImageInDescriptorSets(const Texture& texture, const uint32_t& binding);
	void UpdateImageInDescriptorSets(const VkSampler& sampler, const VkImageView& imageView, const uint32_t& binding);
};