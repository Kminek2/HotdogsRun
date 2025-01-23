#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Image.h"
#include "RenderPass.h"
#include "Framebuffer.h"

#include <vector>

class SwapChain
{
private:
	static VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	Images* swapChainImages;

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	RenderPass* renderPass;
	std::vector<Framebuffer*> framebuffers;

	void CreateSurface();

	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void CreateSwapChain();
	void CreateImageViews();

	void CleanUp();

	void CreateFrameBuffers();
public:
	uint16_t width, height;
	friend RenderPass;

	SwapChain(uint16_t width, uint16_t height);
	~SwapChain();

	static VkSurfaceKHR& getSurface() { return surface; }
	VkExtent2D getExtend() const { return swapChainExtent; }
	VkSwapchainKHR getSwapChain() { return swapChain; }

	RenderPass* getRenderPass() const { return renderPass; }
	Framebuffer* getFramebuffer(unsigned int frame) { return framebuffers[frame]; }
	VkImage* getImage(uint16_t frame) const { return &swapChainImages->images[frame]; }

	void CreateWholeSwapChain();

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	void ReCreate();
};

