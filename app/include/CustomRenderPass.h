#pragma once
#include "Shader.h"
#include "Uniform.h"
#include "AutomaticBuffer.h"

class CustomRenderPass // Render pass 2.0
{
private:
	std::vector<std::pair<VkAttachmentDescription, VkAttachmentReference>> attachments;
	VkRenderPass renderPass;
	static VkFormat FindDepthFormat();
	Uniform* uniform;

	void MakeGraphicsPipeline(std::vector<std::pair<std::string, VkShaderStageFlagBits>> shadersPathAndStage);
	std::pair<VkAttachmentDescription, VkAttachmentReference> CreateDepthAttachment();
public:
	CustomRenderPass();
	~CustomRenderPass();

	void AddBinding(VkShaderStageFlagBits shaderStage, VkDescriptorType bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	template<typename T>
	void AddBinding(AutomaticBuffer<T>* buffer, VkShaderStageFlagBits shaderStage, VkDescriptorType bindingType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	void AddDepthSubpass(std::string vertexShaderPath);
	void CreateRenderPass();
};

template<typename T>
inline void CustomRenderPass::AddBinding(AutomaticBuffer<T>* buffer, VkShaderStageFlagBits shaderStage, VkDescriptorType bindingType)
{
	AddBinding(shaderStage, bindingType);
	buffer->SetBinding(uniform, uniform->getBindingCount());
}
