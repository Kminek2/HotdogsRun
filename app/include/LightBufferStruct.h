#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct DirLightBuffer {
	alignas(16) glm::vec3 dir;

	alignas(16) glm::vec3 ambient;
	alignas(16) glm::vec3 diffuse;
	alignas(16) glm::vec3 specular;
};

struct PointLightBuffer {
	alignas(16) glm::vec3 pos;
	alignas(16) glm::vec3 col;
	alignas(16) glm::vec3 functions;
};

struct SpotLightBuffer {
	alignas(16) glm::vec3 pos;
	alignas(16) glm::vec3 col;
	alignas(16) glm::vec3 functions;

	alignas(16) glm::vec3 direction;
	glm::vec2 cutOffs;
};