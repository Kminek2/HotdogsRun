#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <map>

class Application;

class Input
{
private:
	static GLFWwindow* window;
	static uint16_t width, height;
	static double lastX, lastY;
	static uint16_t* keyEventMap;
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	static double mousePosX, mousePosY;
	static double mouseOffX, mouseOffY;
	static bool getKeyPressed(uint16_t keyKode);
	static bool getKeyReleased(uint16_t keyKode);
	static bool getKeyClicked(uint16_t keyKode);
	static void startKeyCallback();
	static void stopKeyCallback();


	friend Application;
};

