#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Image.h"
#include "UniformBuffer.h"

class Descriptior;

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
	~Uniform();

	template <typename T>
	void UpdateDescriptorSets(const UniformBuffer<T>& buffer, uint32_t binding, VkDeviceSize size);
	void UpdateImageInDescriptorSets(const Texture& texture, const uint32_t& binding);
	friend Descriptior;
};

template<typename T>
inline void Uniform::UpdateDescriptorSets(const UniformBuffer<T>& buffer, uint32_t binding, VkDeviceSize size)
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
		descriptorWrite.descriptorType = types[binding];
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(Device::getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}
