#pragma once
#include "Shader.h"
#include "Uniform.h"
#include "AutomaticBuffer.h"

class CustomRenderPass // Render pass 2.0
{
private:
	VkRenderPass renderPass;
	static VkFormat FindDepthFormat();
	std::vector< VkAttachmentDescription> attachments{};
	Uniform* uniform;
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
