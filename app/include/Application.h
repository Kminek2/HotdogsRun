#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "SoundEngine.h"
#include <list>
#include <utility>
#include <thread>
#include <chrono>

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

	template<typename Function, typename... Args>
	static void Invoke(Function&& func, unsigned long long delayMs, Args&&... args);

	static void LoadScene(std::string scene);
	static void Quit();

	static uint16_t width, height;

	friend Engine;
};

template<typename Function, typename ...Args>
inline void Application::Invoke(Function&& func, unsigned long long delayMs, Args && ...args)
{
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMs)); // Sleep for the specified delay
		std::invoke(func, args...); // Call the function with arguments
	}).detach(); // Detach the thread to run independently
}
