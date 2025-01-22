#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "RenderPass.h"

class SwapChain;

class Framebuffer
{
private:
	VkFramebuffer framebuffer;
public:
	~Framebuffer();
	void CreateFramebuffer(VkImageView swapChainImageView, RenderPass* renderPass, SwapChain* swapChain);

	VkFramebuffer& getFramebuffer() { return framebuffer; }
};

