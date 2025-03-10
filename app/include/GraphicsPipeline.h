#pragma once
#include "Uniform.h"
#include <string>

class RenderPass;

class GraphicsPipeline
{
private:
	Uniform* uniform;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
public:
	GraphicsPipeline(std::string vetrexShaderPath, std::string fragmentShaderPath, uint16_t subPass, RenderPass& renderPass, VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	~GraphicsPipeline();

	VkPipeline& getPipeline() { return graphicsPipeline; }
	VkPipelineLayout& getPipelineLayout() { return pipelineLayout; }
};

