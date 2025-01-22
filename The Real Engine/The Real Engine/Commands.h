#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "SwapChain.h"
class Engine;

class Commands
{
private:
	static VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	void ResetCommands(uint16_t frame);
	static void TransitionToPresent(VkCommandBuffer commandBuffer, VkImage& swapchainImage);
public:
	Commands();
	~Commands();
	void CreateCommandBuffers(const uint16_t& MAX_FRAMES_IN_FLIGHT);
	void RecordCommands(uint16_t frame, const VkFramebuffer& framebuffer, const SwapChain& swapChain, uint16_t currentImage);

	static VkCommandBuffer BeginSingleTimeCommands();

    static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	friend Engine;
};