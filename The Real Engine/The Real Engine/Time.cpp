#include "Time.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

float Time::lastTime;
float Time::deltaTime;

void Time::UpdateTime()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastTime;
	lastTime = currentFrame;
}
