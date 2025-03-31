#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct DirLightBuffer {
	glm::vec3 dir;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLightBuffer {
	alignas(16) glm::vec3 pos;
	alignas(16) glm::vec3 col;
	alignas(16) glm::vec3 functions;
};

struct SpotLightBuffer {
	glm::vec2 cutOffs;
	alignas(16) glm::vec3 pos;
	alignas(16) glm::vec3 col;
	alignas(16) glm::vec3 functions;

	alignas(16) glm::vec3 direction;
};