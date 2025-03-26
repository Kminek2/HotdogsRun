#include "CustomRenderPass.h"

void CustomRenderPass::CreateCompute(std::string shaderPath)
{
	Shader compute(shaderPath, VK_SHADER_STAGE_COMPUTE_BIT);
}

void CustomRenderPass::AddComputeBinding(VkDescriptorType bindingType)
{
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = layoutBindings.size();
	layoutBinding.descriptorCount = 1;
	layoutBinding.descriptorType = bindingType;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

	layoutBindings.push_back(layoutBinding);
}

CustomRenderPass::CustomRenderPass(RenderPassFunction function) : function(function)
{
}

void CustomRenderPass::AddBinding(VkDescriptorType bindingType)
{
	if (function == COMPUTE)
		AddComputeBinding(bindingType);
}

void CustomRenderPass::CreatePass(std::string shaderPath)
{
	if (function == COMPUTE)
		CreateCompute(shaderPath);

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();

	//if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &computeDescriptorSetLayout) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create compute descriptor set layout!");
	//}  IT SHOULDNT BE THERE?
}
