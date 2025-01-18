#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>

class Engine
{
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    Engine(uint32_t Width = 800, uint32_t Height = 600);
private:
    const uint32_t WIDTH;
    const uint32_t HEIGHT;

    GLFWwindow* window;

    void initVulkan();
    void mainLoop();
    void cleanup();
    void initWindow();
};