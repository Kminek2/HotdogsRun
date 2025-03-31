#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

class Application;

class Input
{
private:
	static GLFWwindow* window;
	static glm::vec2 offset;
	static uint16_t width, height;
	static glm::vec2 lastPos;
	static uint16_t* keyEventMap;
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void handleButtonCallback(GLFWwindow* window, int button, int action, int mods);
public:
	static glm::vec2 mousePos;
	static glm::vec2 mouseOff;
	static glm::vec2 mousePercPos;
	static bool getKeyPressed(uint16_t keyKode);
	static bool getKeyReleased(uint16_t keyKode);
	static bool getKeyClicked(uint16_t keyKode);
	static void startKeyCallback();
	static void stopKeyCallback();


	friend Application;
};

