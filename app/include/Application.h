#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "SoundEngine.h"

#define FRAMES_IN_FLIGHT 2
#define ASPECT_WIDTH 16
#define ASPECT_HEIGHT 8

class Engine;

class Application
{
private:
	Camera* camera;
	SoundEngine* soundEngine;
	static GLFWwindow* window;
public:
	Application(uint16_t width, uint16_t height, GLFWwindow* window);
	~Application();

	void Update();
	void UpdateBuffer(uint16_t frame, Descriptior* descriptor);
	void UpdateWindowSizes(uint16_t width, uint16_t height);

	static void LoadScene(std::string scene);
	static void Quit();

	static uint16_t width, height;

	friend Engine;
};

