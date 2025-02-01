#include "Input.h"

GLFWwindow* Input::window;
uint16_t Input::width, Input::height;
glm::vec2 Input::lastPos = glm::vec2(400, 300);
glm::vec2 Input::mousePos, Input::mousePercPos, Input::mouseOff;
uint16_t* Input::keyEventMap = nullptr;

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos = glm::vec2(xpos, ypos);
	mousePercPos = mousePos / glm::vec2(width, height);
	mouseOff = glm::vec2(xpos - lastPos.x, ypos - lastPos.y);

	lastPos = glm::vec2(xpos, ypos);
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