#include "RenderPass.h"
#include <array>
#include "SwapChain.h"
#include "Device.h"
#include <iostream>
#include "Camera.h"
#include "Model.h"
#include "LightObject.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Uniform.h"

RenderPass::RenderPass(SwapChain* swapChain)
{
    this->swapChain = swapChain;
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChain->swapChainImageFormat;
    colorAttachment.samples = Device::GetSampleCount();
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChain->swapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 3;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = Device::GetSampleCount();
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
    uiDepthAttachment.samples = Device::GetSampleCount();
    uiDepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    uiDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    uiDepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    uiDepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    uiDepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    uiDepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference uiDepthAttachmentRef{};
    uiDepthAttachmentRef.attachment = 2;
    uiDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    /*VkAttachmentDescription shadowDepthAttachment{};
    shadowDepthAttachment.format = FindDepthFormat();
    shadowDepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    shadowDepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    shadowDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    shadowDepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    shadowDepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    shadowDepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    shadowDepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference shadowDepthAttachmentRef{};
    shadowDepthAttachmentRef.attachment = 3;
    shadowDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference shadowDepthAttachmentRef2{};
    shadowDepthAttachmentRef.attachment = 3;
    shadowDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;*/

    std::array<VkSubpassDescription, 5> subpasses{};
    /*subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].colorAttachmentCount = 0;
    subpasses[0].pDepthStencilAttachment = &shadowDepthAttachmentRef;
    //subpasses[0].pResolveAttachments = &shadowDepthAttachmentRef2;*/

    subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].colorAttachmentCount = 1;
    subpasses[0].pColorAttachments = &colorAttachmentRef;
    subpasses[0].pDepthStencilAttachment = &depthAttachmentRef;

    subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[1].colorAttachmentCount = 1;
    subpasses[1].pColorAttachments = &colorAttachmentRef;
    subpasses[1].pDepthStencilAttachment = &depthAttachmentRef;

    subpasses[2].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[2].colorAttachmentCount = 1;
    subpasses[2].pColorAttachments = &colorAttachmentRef;
    //subpasses[1].pDepthStencilAttachment = nullptr;
    subpasses[2].pDepthStencilAttachment = &uiDepthAttachmentRef;
    //subpasses[1].inputAttachmentCount = 1;
    //subpasses[1].pInputAttachments = &inputAttachmentRef;

    subpasses[3].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[3].colorAttachmentCount = 1;
    subpasses[3].pColorAttachments = &colorAttachmentRef;
    subpasses[3].pDepthStencilAttachment = &uiDepthAttachmentRef;

    subpasses[4].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[4].colorAttachmentCount = 1;
    subpasses[4].pColorAttachments = &colorAttachmentRef;
    subpasses[4].pDepthStencilAttachment = &depthAttachmentRef;

    subpasses[subpasses.size() - 1].pResolveAttachments = &colorAttachmentResolveRef;

    std::array<VkSubpassDependency, 5> dependencies{};
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = 0;
    dependencies[0].dstStageMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = 1;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    dependencies[2].srcSubpass = 1;
    dependencies[2].dstSubpass = 2;
    dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[2].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[2].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    dependencies[3].srcSubpass = 2;
    dependencies[3].dstSubpass = 3;
    dependencies[3].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[3].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[3].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[3].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    dependencies[4].srcSubpass = 3;
    dependencies[4].dstSubpass = 4;
    dependencies[4].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[4].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[4].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[4].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 4> attachments = { colorAttachment, depthAttachment, uiDepthAttachment, colorAttachmentResolve };
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

    //SHADOW
    /*std::vector<GraphicsPipeline::BindingStruct> shadowBindings = {
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (CubeMap*)nullptr, LightObject::dirLightShadow->lightMatBuffer->GetBuffer(), LightObject::dirLightShadow->lightMatBuffer->getSize())
    };

    std::vector<VkVertexInputBindingDescription> shadowBindingDescriptions = { Vertex::GetPosBindingDescription(0), Transform::GetBindingDescription(1)};

    std::vector<VkVertexInputAttributeDescription> shadowAttributeDescriptions = { Vertex::GetPosAttributeDescriptions(0, 0) };
    std::vector<VkVertexInputAttributeDescription> transformDescriptionsSh = Transform::GetAttributeDescriptions(1, 1);

    shadowAttributeDescriptions.insert(shadowAttributeDescriptions.end(), transformDescriptionsSh.begin(), transformDescriptionsSh.end());*/
    //MAIN
    std::vector<GraphicsPipeline::BindingStruct> mainBindings = {
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (CubeMap*)nullptr, Camera::main->getBuffer()->GetBuffer(), Camera::main->getBuffer()->getSize()),
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, Model::textures, nullptr, 0),
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, (CubeMap*)nullptr, LightObject::getPointBuffer()->GetBuffer(), LightObject::getPointBuffer()->getSize()),
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, (CubeMap*)nullptr, LightObject::getSpotBuffer()->GetBuffer(), LightObject::getSpotBuffer()->getSize()),
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, (CubeMap*)nullptr, GameObject::allColorChanges->GetBuffer(), GameObject::allColorChanges->getSize()),
//       GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, LightObject::dirLightShadow->depthImageSampler, LightObject::dirLightShadow->lightDepthImage.imageView),
//        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (CubeMap*)nullptr, LightObject::dirLightShadow->lightMatBuffer->GetBuffer(), LightObject::dirLightShadow->lightMatBuffer->getSize())
    };

    std::vector<VkVertexInputBindingDescription> mainBindingDescriptions = { Vertex::GetBindingDescription(0), Transform::GetBindingDescription(1), Model::GetBindingDescription(2), ThisColorChanges::GetBindingDescription(3)};
    std::vector<VkVertexInputAttributeDescription> mainAttributeDescriptions = Vertex::GetAttributeDescriptions(0);
    std::vector<VkVertexInputAttributeDescription> transformDescriptions = Transform::GetAttributeDescriptions(1, 3);
    VkVertexInputAttributeDescription textureDescriptions = Model::GetAttributeDescriptions(2, 7);
    std::vector<VkVertexInputAttributeDescription> colorAttributes = ThisColorChanges::GetAttributeDescriptions(3, 8);
    mainAttributeDescriptions.insert(mainAttributeDescriptions.end(), transformDescriptions.begin(), transformDescriptions.end());

    mainAttributeDescriptions.push_back(textureDescriptions);

    mainAttributeDescriptions.insert(mainAttributeDescriptions.end(), colorAttributes.begin(), colorAttributes.end());
    //CUBEMAP
    std::vector<GraphicsPipeline::BindingStruct> cubeMapBindings = {
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, (CubeMap*)nullptr, nullptr, 0),
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, (CubeMap*)nullptr, Camera::main->getBuffer()->GetBuffer(), Camera::main->getBuffer()->getSize())
    };

    std::vector<VkVertexInputBindingDescription> cubeMapBindingDescriptions = { Vertex::GetBindingDescription(0) };
    std::vector<VkVertexInputAttributeDescription> cubeMapAttributeDescriptions = Vertex::GetAttributeDescriptions(0);
    //SPRITE
    std::vector<GraphicsPipeline::BindingStruct> spriteBindings = {
        GraphicsPipeline::BindingStruct(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, Sprite::textures, nullptr, 0),
    };
    
    std::vector<VkVertexInputBindingDescription> spriteBindingDescriptions = { Vertex::GetBindingDescription(0), Sprite::SpriteSendData::GetBindingDescription(1), Transform::GetBindingDescription(2)};
    std::vector<VkVertexInputAttributeDescription> spriteAttributeDescriptions = Vertex::GetAttributeDescriptions(0, 0);
    
    std::vector<VkVertexInputAttributeDescription> spAtrib = Sprite::SpriteSendData::GetAttributeDescriptions(1, 3);
    std::vector<VkVertexInputAttributeDescription> trAtrib = Transform::GetAttributeDescriptions(2, 6);
    spriteAttributeDescriptions.insert(spriteAttributeDescriptions.end(), spAtrib.begin(), spAtrib.end());
    spriteAttributeDescriptions.insert(spriteAttributeDescriptions.end(), trAtrib.begin(), trAtrib.end());

    //shadowPipeline = new GraphicsPipeline("app/shaders/shadows.vert.spv", "", 0, *this, VK_FRONT_FACE_COUNTER_CLOCKWISE, shadowBindings, shadowBindingDescriptions, shadowAttributeDescriptions);
    //std::cout << "Created shadow pipeline";
    mainPipeline = new GraphicsPipeline("app/shaders/main.vert.spv", "app/shaders/main.frag.spv", 0, *this, VK_FRONT_FACE_COUNTER_CLOCKWISE, mainBindings, mainBindingDescriptions, mainAttributeDescriptions);
    std::cout << "Created main pipeline\n";
    cubeMapPipeline = new GraphicsPipeline("app/shaders/CubeMap.vert.spv", "app/shaders/CubeMap.frag.spv", 1, *this, VK_FRONT_FACE_CLOCKWISE, cubeMapBindings, cubeMapBindingDescriptions, cubeMapAttributeDescriptions);
    std::cout << "Created cube map pipeline - yes It's just for cubeMap :)\n";
    UIPipeline = new GraphicsPipeline("app/shaders/ui.vert.spv", "app/shaders/ui.frag.spv", 2, *this, VK_FRONT_FACE_COUNTER_CLOCKWISE, {}, mainBindingDescriptions, mainAttributeDescriptions, mainPipeline->GetUniform());
    std::cout << "Created UI pipeline\n";
    SpritePipeline = new GraphicsPipeline("app/shaders/Sprite.vert.spv", "app/shaders/Sprite.frag.spv", 3, *this, VK_FRONT_FACE_COUNTER_CLOCKWISE, spriteBindings, spriteBindingDescriptions,  spriteAttributeDescriptions);
    std::cout << "Created Sprite Pipeline\n";
    debugingPipeline = new GraphicsPipeline("app/shaders/debug.vert.spv", "app/shaders/ui.frag.spv", 4, *this, VK_FRONT_FACE_COUNTER_CLOCKWISE, {}, mainBindingDescriptions, mainAttributeDescriptions, mainPipeline->GetUniform(), VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
    std::cout << "Created debbuging pipeline\n";


    CreateDepthResources();
    std::cout << "Created depth image";
}

RenderPass::~RenderPass() {
    delete mainPipeline;
    delete UIPipeline;
    delete SpritePipeline;
    delete debugingPipeline;
    vkDestroyRenderPass(Device::getDevice(), renderPass, nullptr);

    DestroyDepthResource();
}

Image RenderPass::getShadowDepthImage()
{
    return LightObject::dirLightShadow->lightDepthImage;
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

    depthImage.CreateImage(swapChain->getExtend().width, swapChain->getExtend().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage.image, depthImageMemory, Device::GetSampleCount());
    depthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    uiDepthImage.CreateImage(swapChain->getExtend().width, swapChain->getExtend().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, uiDepthImage.image, uiDepthImageMemory, Device::GetSampleCount());
    uiDepthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

//    LightObject::dirLightShadow->RecreateDepthImage(glm::uvec2(swapChain->getExtend().width, swapChain->getExtend().height));
//    mainPipeline->GetUniform()->UpdateImageInDescriptorSets(LightObject::dirLightShadow->depthImageSampler, LightObject::dirLightShadow->lightDepthImage.imageView, 5);
}