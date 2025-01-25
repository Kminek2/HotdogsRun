#include "Input.h"

GLFWwindow* Input::window;
uint16_t Input::width, Input::height;
double Input::lastX = 400, Input::lastY = 300;
double Input::mousePosX, Input::mousePosY;
double Input::mouseOffX, Input::mouseOffY;

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mousePosX = xpos;
	mousePosY = ypos;

	mouseOffX = xpos - lastX;
	mouseOffY = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
}

bool Input::getKeyPressed(uint16_t keyKode)
{
	return glfwGetKey(window, keyKode) == GLFW_PRESS;
}
