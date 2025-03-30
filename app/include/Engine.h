#pragma once
#define GLFW_INCLUDE_VULKAN
#define NVALID 1
#include <GLFW/glfw3.h>

#include "SwapChain.h"
#include "Device.h"
#include "ValidationLayers.h"
#include "Commands.h"
#include "Application.h"

#include <cstdint>
#include <stdexcept>

class Engine
{
public:
    void Run() {
        MainLoop();
        Cleanup();
    }

    Engine(uint32_t Width = 800, uint32_t Height = 600);

    static GLFWwindow* window;
    static VkInstance instance;

    static ValidationLayers* getValidationLayers() { return enableValidationLayers ? validationLayers : nullptr; };
    static const std::vector<const char*> enValidationLayers;
private:
    const uint32_t WIDTH;
    const uint32_t HEIGHT;
#ifdef NVALID
    static const bool enableValidationLayers = false;
#else
    static const bool enableValidationLayers = true;
#endif //NDEBUG

    static ValidationLayers* validationLayers;
    SwapChain* swapChain;
    Device* device;
    Commands* commands;
    Application* application;

    void InitVulkan();
    void MainLoop();
    void Cleanup();
    void InitWindow();

    static VkInstance& getInstance() { return instance; };
    static GLFWwindow* getWindow() { return window; };
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

    void CreateInstance();
    void CreateSyncObjects();
    std::vector<const char*> GetRequiredExtensions();

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    uint32_t currentFrame = 0;

    bool framebufferResized;

    void DrawFrame();
};