#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Descriptior;

class Uniform
{
private:
	std::vector<VkDescriptorSetLayoutBinding> UBOs;
	static VkDescriptorSetLayout descriptorSetLayout;
public:
	void AddUniforms(uint16_t amount = 1);
	std::vector<VkDescriptorSetLayout> BindUniforms();
	~Uniform();

	friend Descriptior;
};