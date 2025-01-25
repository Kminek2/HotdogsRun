#include "Input.h"

GLFWwindow* Input::window;
uint16_t Input::width, Input::height;
double Input::lastX = 400, Input::lastY = 300;
double Input::mousePosX, Input::mousePosY;
double Input::mouseOffX, Input::mouseOffY;
uint16_t* Input::keyEventMap = nullptr;

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
	if (keyEventMap == nullptr || keyKode > GLFW_KEY_LAST || keyKode < 0)
		return false;
	return keyEventMap[keyKode] > 0;
}

bool Input::getKeyReleased(uint16_t keyKode)
{
	if (keyEventMap == nullptr || keyKode > GLFW_KEY_LAST || keyKode < 0)
		return true;
	return keyEventMap[keyKode] == 0;
}

bool Input::getKeyClicked(uint16_t keyKode)
{
	if (keyEventMap == nullptr || keyKode > GLFW_KEY_LAST || keyKode < 0)
		return false;

	if (keyEventMap[keyKode] != 1)
		return false;

	keyEventMap[keyKode] = 2;
	return true;
}

void Input::startKeyCallback() {
	if (keyEventMap != nullptr) {
		delete[] keyEventMap;
		keyEventMap = nullptr;
	}

	keyEventMap = new uint16_t[GLFW_KEY_LAST+1];

	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
		keyEventMap[i] = 0;

	glfwSetKeyCallback(window, handleKeyCallback);
}

void Input::stopKeyCallback() {
	if (keyEventMap != nullptr) {
		delete[] keyEventMap;
		keyEventMap = nullptr;
	}

	glfwSetKeyCallback(window, NULL);
}

void Input::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (keyEventMap == nullptr || key > GLFW_KEY_LAST || key < 0)
		return;

	if (action == GLFW_PRESS)
		keyEventMap[key] = 1;
	else if (action == GLFW_RELEASE)
		keyEventMap[key] = 0;
}