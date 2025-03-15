#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "GraphicsPipeline.h"

#include "Image.h"

class SwapChain;

class RenderPass
{
private:
	SwapChain* swapChain;
	VkRenderPass renderPass;
	GraphicsPipeline* mainPipeline;
	GraphicsPipeline* cubeMapPipeline;
	GraphicsPipeline* UIPipeline;
	GraphicsPipeline* SpritePipeline;
	GraphicsPipeline* debugingPipeline;

	Image depthImage;
	VkDeviceMemory depthImageMemory;

	Image uiDepthImage;
	VkDeviceMemory uiDepthImageMemory;
	static VkFormat FindDepthFormat();

	void CreateDepthResources();
	void DestroyDepthResource();
public:
	RenderPass(SwapChain* swapChain);
	~RenderPass();

	VkRenderPass getRenderPass() { return renderPass; }
	Image getDepthImage() const { return depthImage; }
	Image getUIDepthImage() const { return uiDepthImage; }
	GraphicsPipeline* getMainPipeline() const { return mainPipeline; }
	GraphicsPipeline* getCubeMapPipeline() const { return cubeMapPipeline; }
	GraphicsPipeline* getUiPipeline() const { return UIPipeline; }
	GraphicsPipeline* getSpritePipeline() const { return SpritePipeline; }
	GraphicsPipeline* getDebugPipeline() const { return debugingPipeline; }

	void RecreateDepthResource();
};

