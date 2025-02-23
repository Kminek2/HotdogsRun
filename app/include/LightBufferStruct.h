#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


struct LightBufferStruct
{
	glm::vec3 directionalLight;
	glm::vec3 directionalLightColor;

	uint32_t lightCount;
	std::vector<std::pair<glm::vec3, uint32_t>> colorAndRadius;
};