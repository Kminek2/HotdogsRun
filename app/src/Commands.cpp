#include "Commands.h"
#include "Queues.h"
#include <stdexcept>

#include <array>
#include "Model.h"
#include "Transform.h"

#include "Descriptior.h"
#include "LightObject.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "LightBufferStruct.h"

#include "Camera.h"
#include "DebugPoints.h"
#include "Application.h"

VkCommandPool Commands::commandPool;

Commands::Commands() {
    Queues::QueueFamilyIndices queueFamilyIndices = Queues::FindQueueFamilies(Device::getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(Device::getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

Commands::~Commands() {
    vkDestroyCommandPool(Device::getDevice(), commandPool, nullptr);
}

void Commands::CreateCommandBuffers(const uint16_t& MAX_FRAMES_IN_FLIGHT) {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    if (vkAllocateCommandBuffers(Device::getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Commands::RecordCommands(uint16_t frame, const VkFramebuffer& framebuffer, const SwapChain& swapChain, uint16_t currentImage)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    VkCommandBuffer commandBuffer = commandBuffers[frame];

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain.getRenderPass()->getRenderPass();
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChain.getExtend();

    std::array<VkClearValue, 3> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };
    clearValues[2].depthStencil = { 1.0f, 0 };

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getMainPipeline()->getPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        //viewport.width = static_cast<float>(swapChain.getExtend().width);
        //viewport.height = static_cast<float>(swapChain.getExtend().height);
        viewport.width = static_cast<float>(Application::width);
        viewport.height = static_cast<float>(Application::height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChain.getExtend();
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { Model::vertexBuffer->getBuffer(),  Transform::transformBuffer->getBuffer(), Model::textureOffBuffer->getBuffer()};
        VkDeviceSize offsets[] = { 0, 0, 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 3, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer, Model::indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getMainPipeline()->getPipelineLayout(), 0, 1, &Descriptior::descriptorSets[frame], 0, nullptr);

        struct PushConstantData {
            glm::vec4 dir = glm::vec4(LightObject::dirLight.dir, 0);
            glm::vec4 ambient = glm::vec4(LightObject::dirLight.ambient, 0);
            glm::vec4 difffuse = glm::vec4(LightObject::dirLight.diffuse, 0);
            glm::vec4 specular = glm::vec4(LightObject::dirLight.specular, 0);
            glm::vec4 cPos = glm::vec4(Camera::main->cameraTransform->position, 0);

            uint32_t pLNum = PointLight::lightNum;
            uint32_t sLNum = SpotLight::lightNum;
            uint32_t padding[2];
        } sendData;
        uint32_t offset = 0;
        uint32_t size = sizeof(sendData);

        vkCmdPushConstants(commandBuffer, swapChain.getRenderPass()->getMainPipeline()->getPipelineLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, offset, size, &sendData);

        uint32_t instanceOff = 0;
        std::pair<std::list<Model*>::iterator, uint32_t> instance;
        while (instanceOff < Model::createdModels.size())
        {
            Model* model = *std::next(Model::createdModels.begin(), instanceOff);
            const auto& instance = Model::modelsIndtaces[model->modelName];

            vkCmdDrawIndexed(commandBuffer, model->indexSize, instance.second, model->indexOffset, model->vertexOffset, instanceOff);
            instanceOff += instance.second;
        }
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getUiPipeline()->getPipeline());

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        vkCmdBindVertexBuffers(commandBuffer, 0, 3, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, Model::indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getUiPipeline()->getPipelineLayout(), 0, 1, &Descriptior::descriptorSets[frame], 0, nullptr);

        vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
        while (instanceOff < Model::createdModels.size() + Model::createdUiModels.size())
        {
            Model* model = *std::next(Model::createdUiModels.begin(), instanceOff - Model::createdModels.size());
            const auto& instance = Model::uiModelsIndtaces[model->modelName];

            vkCmdDrawIndexed(commandBuffer, model->indexSize, instance.second, model->indexOffset, model->vertexOffset, instanceOff);
            instanceOff += instance.second;
        }

        if (DebugPoints::indicies.size() > 0) {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getDebugPipeline()->getPipeline());

            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, &DebugPoints::vertexBuffer->getBuffer(), offsets);
            vkCmdBindIndexBuffer(commandBuffer, DebugPoints::indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, swapChain.getRenderPass()->getDebugPipeline()->getPipelineLayout(), 0, 1, &Descriptior::descriptorSets[frame], 0, nullptr);

        }
        vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
        if (DebugPoints::indicies.size() > 0)
            vkCmdDrawIndexed(commandBuffer, DebugPoints::indicies.size(), 1, 0, 0, 0);
    }

    vkCmdEndRenderPass(commandBuffer);

    //TransitionToPresent(commandBuffer, *swapChain.getImage(currentImage));

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Commands::ResetCommands(uint16_t frame)
{
    vkResetCommandBuffer(commandBuffers[frame], 0);
}

VkCommandBuffer Commands::BeginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Device::getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Commands::EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Queues::graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Queues::graphicsQueue);

    vkFreeCommandBuffers(Device::getDevice(), commandPool, 1, &commandBuffer);
}

void Commands::TransitionToPresent(VkCommandBuffer commandBuffer, VkImage& swapchainImage) {
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Adjust based on your use case
    barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = swapchainImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = 0;

    vkCmdPipelineBarrier(
        commandBuffer,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0, 0, nullptr, 0, nullptr, 1, &barrier
    );
}