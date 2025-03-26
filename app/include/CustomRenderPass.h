#pragma once
#include "Shader.h"
class CustomRenderPass
{
public:
	enum RenderPassFunction {
		RENDER,
		COMPUTE
	};

private:
	const RenderPassFunction function;
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};


	void CreateCompute(std::string shaderPath);
	void AddComputeBinding(VkDescriptorType bindingType);
public:
	CustomRenderPass(RenderPassFunction function);

	void AddBinding(VkDescriptorType bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	void CreatePass(std::string shaderPath);
};