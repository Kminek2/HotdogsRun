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
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 functions;
};

struct SpotLightBuffer {
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 functions;

	glm::vec3 direction;
	glm::vec2 cutOffs;
};