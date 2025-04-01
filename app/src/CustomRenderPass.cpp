#include "CustomRenderPass.h"

VkFormat CustomRenderPass::FindDepthFormat()
{
    return Device::FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

void CustomRenderPass::MakeGraphicsPipeline(std::vector<std::pair<std::string, VkShaderStageFlagBits>> shadersPathAndStage)
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    for (auto& shader : shadersPathAndStage) {
        Shader loadedShader(shader.first, shader.second);
        shaderStages.push_back(loadedShader.getShaderStageInfo());
    }


    std::vector<VkVertexInputBindingDescription> bindingDescriptions = bindingDesc;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = atribDesc;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}

std::pair<VkAttachmentDescription, VkAttachmentReference> CustomRenderPass::CreateDepthAttachment()
{
    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = FindDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 0;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachments.push_back({ depthAttachment, depthAttachmentRef });

    return { depthAttachment, depthAttachmentRef };
}

CustomRenderPass::CustomRenderPass()
{
	uniform = new Uniform();
    renderPass = nullptr;
}

CustomRenderPass::~CustomRenderPass()
{
    if(uniform != nullptr)
	    delete uniform;

    if(renderPass != nullptr)
        vkDestroyRenderPass(Device::getDevice(), renderPass, nullptr);

}

void CustomRenderPass::AddBinding(VkShaderStageFlagBits shaderStage, VkDescriptorType bindingType)
{
	uniform->AddUniforms(1U, bindingType, shaderStage);
}

void CustomRenderPass::AddDepthSubpass(std::string vertexShaderPath)
{
    auto depthAttachment = CreateDepthAttachment();

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 0;
    subpass.pDepthStencilAttachment = &depthAttachment.second;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &depthAttachment.first;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(Device::getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create depth render pass!");
    }

    MakeGraphicsPipeline({ {"app/shaders/shadows.vert.spv", VK_SHADER_STAGE_VERTEX_BIT} });
}

void CustomRenderPass::CreateRenderPass()
{
	uniform->BindUniforms();
}
