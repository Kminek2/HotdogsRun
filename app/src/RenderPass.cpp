#include "RenderPass.h"
#include <array>
#include "SwapChain.h"
#include "Device.h"
#include <iostream>

RenderPass::RenderPass(SwapChain* swapChain)
{
    this->swapChain = swapChain;
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain->swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription uiDepthAttachment{};
    uiDepthAttachment.format = FindDepthFormat();
    uiDepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    uiDepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    uiDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    uiDepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    uiDepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    uiDepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    uiDepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference uiDepthAttachmentRef{};
    uiDepthAttachmentRef.attachment = 2;
    uiDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array<VkSubpassDescription, 3> subpasses{};
    subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].colorAttachmentCount = 1;
    subpasses[0].pColorAttachments = &colorAttachmentRef;
    subpasses[0].pDepthStencilAttachment = &depthAttachmentRef;

    subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[1].colorAttachmentCount = 1;
    subpasses[1].pColorAttachments = &colorAttachmentRef;
    //subpasses[1].pDepthStencilAttachment = nullptr;
    subpasses[1].pDepthStencilAttachment = &uiDepthAttachmentRef;
    //subpasses[1].inputAttachmentCount = 1;
    //subpasses[1].pInputAttachments = &inputAttachmentRef;

    subpasses[2].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[2].colorAttachmentCount = 1;
    subpasses[2].pColorAttachments = &colorAttachmentRef;
    subpasses[2].pDepthStencilAttachment = &depthAttachmentRef;

    std::array<VkSubpassDependency, 3> dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = 0;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = 1;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    dependencies[2].srcSubpass = 1;
    dependencies[2].dstSubpass = 2;
    dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[2].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[2].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, uiDepthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    if (vkCreateRenderPass(Device::getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

    std::cout << "Created render pass\n";

    mainPipeline = new GraphicsPipeline("app/shaders/main.vert.spv", "app/shaders/main.frag.spv", 0, *this);
    std::cout << "Created main pipeline\n";
    UIPipeline = new GraphicsPipeline("app/shaders/ui.vert.spv", "app/shaders/ui.frag.spv", 1, *this);
    std::cout << "Created UI pipeline\n";
    SpritePipeline = new GraphicsPipeline("app/shaders/Sprite.vert.spv", "app/shaders/Sprite.frag.spv", 2, *this);
    std::cout << "Created Sprite Pipeline\n";
    debugingPipeline = new GraphicsPipeline("app/shaders/debug.vert.spv", "app/shaders/ui.frag.spv", 3, *this, VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
    std::cout << "Created debbuging pipeline\n";


    CreateDepthResources();
    std::cout << "Created depth image";
}

RenderPass::~RenderPass() {
    delete mainPipeline;
    delete UIPipeline;
    delete debugingPipeline;
    vkDestroyRenderPass(Device::getDevice(), renderPass, nullptr);

    DestroyDepthResource();
}

void RenderPass::DestroyDepthResource() {
    vkDestroyImageView(Device::getDevice(), depthImage.imageView, nullptr);
    vkDestroyImage(Device::getDevice(), depthImage.image, nullptr);

    vkDestroyImageView(Device::getDevice(), uiDepthImage.imageView, nullptr);
    vkDestroyImage(Device::getDevice(), uiDepthImage.image, nullptr);

    vkFreeMemory(Device::getDevice(), depthImageMemory, nullptr);
    vkFreeMemory(Device::getDevice(), uiDepthImageMemory, nullptr);
}

void RenderPass::RecreateDepthResource() {
    DestroyDepthResource();
    CreateDepthResources();
}

VkFormat RenderPass::FindDepthFormat() {
    return Device::FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void RenderPass::CreateDepthResources() {
    VkFormat depthFormat = FindDepthFormat();

    depthImage.CreateImage(swapChain->getExtend().width, swapChain->getExtend().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage.image, depthImageMemory);
    depthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    uiDepthImage.CreateImage(swapChain->getExtend().width, swapChain->getExtend().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, uiDepthImage.image, uiDepthImageMemory);
    uiDepthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}