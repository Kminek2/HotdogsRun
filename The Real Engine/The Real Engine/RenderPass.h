#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "GraphicsPipeline.h"

#include "Uniform.h"

#include "Image.h"

class SwapChain;

class RenderPass
{
private:
	SwapChain* swapChain;
	VkRenderPass renderPass;
	GraphicsPipeline* mainPipeline;
	//GraphicsPipeline UIPipeline;

	Image depthImage;
	VkDeviceMemory depthImageMemory;
	static VkFormat FindDepthFormat();

	void CreateDepthResources();

	void DestroyDepthResource();
public:
	RenderPass(SwapChain* swapChain);
	~RenderPass();

	VkRenderPass getRenderPass() { return renderPass; }
	Image getDepthImage() const { return depthImage; }
	GraphicsPipeline* getMainPipeline() const { return mainPipeline; }

	void RecreateDepthResource();
};

