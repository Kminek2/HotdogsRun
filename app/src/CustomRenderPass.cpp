#include "CustomRenderPass.h"

void CustomRenderPass::CreateCompute(std::string shaderPath)
{
	Shader compute(shaderPath, VK_SHADER_STAGE_COMPUTE_BIT);

	uniform = Uniform();
}

void CustomRenderPass::AddComputeBinding(VkDescriptorType bindingType)
{
	uniform.AddUniforms(1U, bindingType, VK_SHADER_STAGE_COMPUTE_BIT);
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

	uniform.BindUniforms();
}
