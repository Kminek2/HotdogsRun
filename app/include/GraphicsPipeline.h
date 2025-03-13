#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class RenderPass;
class Uniform;
class Texture;
template<typename T>
class UniformBuffer;

class GraphicsPipeline
{
private:
	Uniform* uniform;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	bool gotUniform;
public:

	struct BindingStruct
	{
		BindingStruct() {};
		BindingStruct(VkDescriptorType descType, VkShaderStageFlagBits shaderStage, Texture* texture = nullptr, std::vector<VkBuffer>* unfiormBuffer = nullptr, VkDeviceSize size = 0) : descType(descType), shaderStage(shaderStage), texture(texture), unfiormBuffer(unfiormBuffer), size(size) {}
		VkDescriptorType descType;
		VkShaderStageFlagBits shaderStage;

		Texture* texture = nullptr;
		std::vector<VkBuffer>* unfiormBuffer = nullptr;
		VkDeviceSize size = 0;
	};

	GraphicsPipeline(std::string vetrexShaderPath, std::string fragmentShaderPath, uint16_t subPass, RenderPass& renderPass, VkFrontFace front, std::vector<BindingStruct> bindings = {}, std::vector<VkVertexInputBindingDescription> bundingDesc = {}, std::vector<VkVertexInputAttributeDescription> atribDesc = {}, Uniform* createdUniform = nullptr, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	~GraphicsPipeline();

	Uniform* GetUniform() { return uniform; }

	VkPipeline& getPipeline() { return graphicsPipeline; }
	VkPipelineLayout& getPipelineLayout() { return pipelineLayout; }
};