#include "Engine.h"

#include "Descriptior.h"
#include <stdexcept>
#include "Queues.h"
#include "LightObject.h"

#include "Model.h"
#include "PointLight.h"
#include "SpotLight.h"

GLFWwindow* Engine::window;
VkInstance Engine::instance;

ValidationLayers* Engine::validationLayers;
const std::vector<const char*> Engine::enValidationLayers = {};

Engine::Engine(uint32_t Width, uint32_t Height) :WIDTH(Width), HEIGHT(Height)
{
    InitWindow();
    InitVulkan();
}

void Engine::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Wyscigi", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
}

void Engine::InitVulkan() {
    CreateInstance();
    if(enableValidationLayers)
        validationLayers = new ValidationLayers(enValidationLayers);
    swapChain = new SwapChain(WIDTH, HEIGHT);
    device = new Device();
    std::cout << "Found matching device" << '\n';
    commands = new Commands();
    commands->CreateCommandBuffers(2);
    std::cout << "Created command buffers" << '\n';
    application = new Application(WIDTH, HEIGHT, window);
    std::cout << "Initialized application" << '\n';
    swapChain->CreateWholeSwapChain();
    std::cout << "Created swapChain" << '\n';
    //descriptor = new Descriptior(FRAMES_IN_FLIGHT, *application->camera->getBuffer(), *Model::textures, *LightObject::getPointBuffer(), *LightObject::getSpotBuffer());
    //std::cout << "Created descriptors" << '\n';
    CreateSyncObjects();
    std::cout << "Created syncingObjects" << '\n';

    std::cout << "Init succesfull" << "\n\n";
}

void Engine::MainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        application->Update();
        DrawFrame();
    }

    vkDeviceWaitIdle(Device::getDevice());
}

void Engine::Cleanup() {
    for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(Device::getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Device::getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(Device::getDevice(), inFlightFences[i], nullptr);
    }

    delete application;

    delete swapChain;

    delete commands;

    delete device;

    if (enableValidationLayers) {
        delete validationLayers;
    }

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Engine::DrawFrame() {
    vkWaitForFences(Device::getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(Device::getDevice(), swapChain->getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChain->ReCreate();
        application->UpdateWindowSizes(swapChain->width, swapChain->height);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(Device::getDevice(), 1, &inFlightFences[currentFrame]);

    application->UpdateBuffer(currentFrame, swapChain->getRenderPass()->getMainPipeline()->GetUniform(), swapChain->getRenderPass()->getCubeMapPipeline()->GetUniform());

    commands->ResetCommands(currentFrame);
    commands->RecordCommands(currentFrame, swapChain->getFramebuffer(imageIndex)->getFramebuffer(), *swapChain, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commands->commandBuffers[currentFrame];
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(Queues::graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    VkSwapchainKHR swapChains[] = { swapChain->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional
    result = vkQueuePresentKHR(Queues::presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        swapChain->ReCreate();
        application->UpdateWindowSizes(swapChain->width, swapChain->height);
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
}

void Engine::CreateInstance() {
    if (enableValidationLayers && !validationLayers->CheckValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Wyscigi";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "The Real Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(2, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(enValidationLayers.size());
        createInfo.ppEnabledLayerNames = enValidationLayers.data();

        ValidationLayers::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

std::vector<const char*> Engine::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Engine::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Engine::CreateSyncObjects() {
    imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(FRAMES_IN_FLIGHT);
    inFlightFences.resize(FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(Device::getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Device::getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Device::getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}
