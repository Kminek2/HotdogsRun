#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct LightBufferStruct
{
	uint16_t lightCount;
	glm::vec3 color;
};