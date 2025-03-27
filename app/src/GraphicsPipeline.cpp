#include "GraphicsPipeline.h"
#include "Shader.h"
#include "Vertex.h"
#include "Transform.h"
#include "Device.h"
#include "RenderPass.h"

#include <vector>
#include <stdexcept>
#include <filesystem>

#include "Model.h"
#include "LightBufferStruct.h"
#include "Application.h"
#include "LightObject.h"
#include "Uniform.h"

#include "Image.h"

GraphicsPipeline::GraphicsPipeline(std::string vetrexShaderPath, std::string fragmentShaderPath, uint16_t subPass, RenderPass& renderPass, VkFrontFace front, std::vector<BindingStruct> bindings, std::vector<VkVertexInputBindingDescription> bindingDesc, std::vector<VkVertexInputAttributeDescription> atribDesc, Uniform* createdUniform, VkPrimitiveTopology topology) {
    std::cout << "Creating pipeline\n";
    Shader* vertexShader = nullptr;

    if (vetrexShaderPath != "")
        vertexShader = new Shader(vetrexShaderPath, VK_SHADER_STAGE_VERTEX_BIT);

    std::cout << "Loaded vertex shader\n";

    //texturesConst
    uint32_t maxTextures = 0;

    for (const auto& entry : std::filesystem::directory_iterator("models_obj")) {
        maxTextures++;
    }

    VkSpecializationMapEntry specializationEntry{};
    specializationEntry.constantID = 0;
    specializationEntry.offset = 0;
    specializationEntry.size = sizeof(int);

    VkSpecializationInfo specializationInfo{};
    specializationInfo.mapEntryCount = 1;
    specializationInfo.pMapEntries = &specializationEntry;
    specializationInfo.dataSize = sizeof(int);
    specializationInfo.pData = &maxTextures;

    //

    std::cout << "Preparing to load " << maxTextures << " textures\n";

    Shader* fragmentShader = nullptr;

    if (fragmentShaderPath != "")
        fragmentShader = new Shader(fragmentShaderPath, VK_SHADER_STAGE_FRAGMENT_BIT, specializationInfo);

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    if (vetrexShaderPath != "")
        shaderStages.push_back(vertexShader->getShaderStageInfo());
    if(fragmentShaderPath != "")
        shaderStages.push_back(fragmentShader->getShaderStageInfo());

    std::vector<VkVertexInputBindingDescription> bindingDescriptions = bindingDesc;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = atribDesc;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = topology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = front;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = fragmentShader == nullptr ? VK_FALSE : VK_TRUE;
    multisampling.rasterizationSamples = fragmentShader == nullptr ? VK_SAMPLE_COUNT_1_BIT : Device::GetSampleCount();
    multisampling.minSampleShading = .3;
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_SUBTRACT; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = subPass == 0 ? 0 : 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    std::vector<VkDescriptorSetLayout> descriptorSetLayout;

    if (createdUniform == nullptr) {
        uniform = new Uniform();

        for (auto& bind : bindings)
            uniform->AddUniforms(1, bind.descType, bind.shaderStage);
        descriptorSetLayout = uniform->BindUniforms();

        for (int i = 0; i < bindings.size(); i++) {
            BindingStruct bind = bindings[i];
            if (bind.unfiormBuffer != nullptr)
                uniform->UpdateDescriptorSets(*bind.unfiormBuffer, i, bind.size);
            else if (bind.texture != nullptr) {
                uniform->UpdateImageInDescriptorSets(*bind.texture, i);
                bind.texture->SetBinding(uniform, i);
            }
            else if (bind.cubeMap != nullptr)
                uniform->UpdateImageInDescriptorSets(*&bind.cubeMap->sampler, *&bind.cubeMap->imageView, i);
            else if (bind.sampler != nullptr)
                uniform->UpdateImageInDescriptorSets(bind.sampler, bind.imageWiev, i);
        }

        gotUniform = false;
    }
    else {
        uniform = createdUniform;
        descriptorSetLayout = uniform->GetUnfiorms();
        gotUniform = true;
    }

    VkPushConstantRange range = {};
    range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    range.offset = 0;
    range.size = sizeof(uint32_t) * 4 + sizeof(glm::vec4) * 5;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayout.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &range;

    if (vkCreatePipelineLayout(Device::getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional


        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass.getRenderPass();
        pipelineInfo.subpass = subPass;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        if (vkCreateGraphicsPipelines(Device::getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

    if(vertexShader != nullptr)
        delete vertexShader;

    if(fragmentShader != nullptr)
        delete fragmentShader;
}

GraphicsPipeline::~GraphicsPipeline() {
    if(!gotUniform){
        delete uniform;
    }
    vkDestroyPipeline(Device::getDevice(), graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(Device::getDevice(), pipelineLayout, nullptr);
}