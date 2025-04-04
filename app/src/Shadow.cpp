#include "Shadow.h"
#include "Application.h"

Shadow::Shadow(glm::uvec2 size, glm::vec3 dir)
{
    VkFormat depthFormat = Device::FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    lightDepthImage.CreateImage(size.x, size.g, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, lightDepthImage.image, depthImageMemory);
    lightDepthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    Image::CreateSampler(depthImageSampler, VK_FILTER_LINEAR, VK_FILTER_LINEAR);

    this->size = size;

    glm::mat4 lightProj = glm::ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f, -20.0f, 30.0f);
    glm::mat4 upMatrix(1);
    upMatrix = glm::rotate(upMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::mat4 lightView = glm::lookAt(glm::vec3(0, 0, 0), dir, glm::vec3(upMatrix * glm::vec4(dir, 1)));
    lightSpaceMatrix = lightProj * lightView;

    lightMatBuffer = new UniformBuffer<glm::mat4>(FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    lightMatBuffer->UpdateData(lightSpaceMatrix);
}

Shadow::~Shadow()
{
    delete lightMatBuffer;
    vkDestroyImage(Device::getDevice(), lightDepthImage.image, nullptr);
    vkDestroyImageView(Device::getDevice(), lightDepthImage.imageView, nullptr);
    vkDestroySampler(Device::getDevice(), depthImageSampler, nullptr);
    vkFreeMemory(Device::getDevice(), depthImageMemory, nullptr);
}

void Shadow::UpdateLightSpaceMatrix(glm::uvec2 size, glm::vec3 pos, glm::vec3 rotation)
{
    rotation = glm::normalize(rotation);
    
    glm::mat4 lightProj = glm::ortho(-(size.x / 2.0f), size.x / 2.0f, -(size.y / 2.0f), size.y / 2.0f, 1.0f, 20.0f);
    
    //glm::mat4 lightProj = glm::perspective(glm::radians(90.0f), this->size.x / (float)this->size.y, 0.1f, 100.0f);

    glm::vec3 upDir = glm::vec3(0, 0, 1);

    glm::vec3 right = glm::normalize(glm::cross(rotation, upDir));

    glm::vec3 up = glm::normalize(glm::cross(rotation, right));

    glm::mat4 lightView = glm::lookAt(pos - rotation * 5.0f, pos, glm::vec3(0, 0, 1));
    //glm::mat4 lightView = glm::lookAt(-rotation * 20.0f, glm::vec3(0), glm::vec3(0, 0, 1));
    //glm::mat4 lightView = glm::lookAt(pos - Camera::main->cameraTransform->front, pos + Camera::main->cameraTransform->front, glm::vec3(0, 0, 1));

    lightSpaceMatrix = lightProj * lightView;
    lightMatBuffer->UpdateData(lightSpaceMatrix);
}

void Shadow::RecreateDepthImage(glm::uvec2 size)
{
    vkDestroyImage(Device::getDevice(), lightDepthImage.image, nullptr);
    vkDestroyImageView(Device::getDevice(), lightDepthImage.imageView, nullptr);
    vkFreeMemory(Device::getDevice(), depthImageMemory, nullptr);

    VkFormat depthFormat = Device::FindSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    this->size = size;

    lightDepthImage.CreateImage(size.x, size.g, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, lightDepthImage.image, depthImageMemory);
    lightDepthImage.CreateImageView(depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}
