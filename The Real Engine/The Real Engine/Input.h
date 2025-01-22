#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application;

class Input
{
private:
	static GLFWwindow* window;
	static uint16_t width, height;
	static double lastX, lastY;
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
public:
	static double mousePosX, mousePosY;
	static double mouseOffX, mouseOffY;
	static bool getKeyPressed(uint16_t keyKode);

	friend Application;
};

