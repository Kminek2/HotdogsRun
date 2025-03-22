#include "Framebuffer.h"
#include "SwapChain.h"
#include "Device.h"

#include <array>

Framebuffer::~Framebuffer() {
    vkDestroyFramebuffer(Device::getDevice(), framebuffer, nullptr);
}

void Framebuffer::CreateFramebuffer(VkImageView msaaImageView, VkImageView swapChainImageView, RenderPass* renderPass, SwapChain* swapChain) {
    std::array<VkImageView, 5> attachments = {
                msaaImageView,
                renderPass->getDepthImage().imageView,
                renderPass->getUIDepthImage().imageView,
                renderPass->getShadowDepthImage().imageView,
                swapChainImageView
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass->getRenderPass();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = swapChain->getExtend().width;
    framebufferInfo.height = swapChain->getExtend().height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(Device::getDevice(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
    }
}