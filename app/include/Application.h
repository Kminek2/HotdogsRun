#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Camera.h"

#define FRAMES_IN_FLIGHT 2

class Engine;

class Application
{
private:
	Camera* camera;
public:
	Application(uint16_t width, uint16_t height, GLFWwindow* window);
	~Application();

	void Update();
	void UpdateBuffer(uint16_t frame);
	void UpdateCamera(uint16_t width, uint16_t height);

	static void LoadScene(std::string scene);

	static uint16_t width, height;

	friend Engine;
};

