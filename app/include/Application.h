#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "SoundEngine.h"
#include "Settings.h"
#include <list>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <filesystem>

#define FRAMES_IN_FLIGHT 2
#define ASPECT_WIDTH 16
#define ASPECT_HEIGHT 9

class Engine;
class Uniform;

class Application
{
private:
	Camera* camera;
	SoundEngine* soundEngine;
	static GLFWwindow* window;

	std::vector<std::filesystem::directory_entry> entries;
	unsigned loadedModel;
	bool loadedAll;
	std::thread modelLoading;
public:
	Application(uint16_t width, uint16_t height, GLFWwindow* window);
	~Application();

	void Update();
	void UpdateBuffer(uint16_t frame, Uniform* uniform, Uniform* cubeMapUniform);
	void UpdateWindowSizes(uint16_t width, uint16_t height);

	template<typename Function, typename... Args>
	static void Invoke(Function&& func, unsigned long long delayMs, Args&&... args);

	static void SetCursor(bool enabled);
	static void LoadScene(std::string scene);
	static void Quit();

	static uint16_t width, height;

	friend Engine;
};

template<typename Function, typename ...Args>
inline void Application::Invoke(Function&& func, unsigned long long delayMs, Args && ...args)
{
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
		std::invoke(func, args...);
	}).detach();
}
