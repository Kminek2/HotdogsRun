#pragma once
#include <glm/glm.hpp>

enum ViewType {
	Perspective,
	Isometric
};

struct UniformCameraBuffer
{
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};