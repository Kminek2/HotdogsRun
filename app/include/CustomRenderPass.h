#pragma once
#include "Shader.h"
#include "Uniform.h"

class CustomRenderPass // Render pass 2.0
{
public:
	enum RenderPassFunction {
		RENDER,
		COMPUTE
	};

private:
	const RenderPassFunction function;
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
	Uniform uniform;

	void CreateCompute(std::string shaderPath);
	void AddComputeBinding(VkDescriptorType bindingType);
public:
	CustomRenderPass(RenderPassFunction function);

	void AddBinding(VkDescriptorType bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	void CreatePass(std::string shaderPath);
};