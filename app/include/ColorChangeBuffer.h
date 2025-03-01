#pragma once
#include "glm/glm.hpp"

struct ColorChangeBuffer {
	alignas(16) glm::vec3 fromCol;
	alignas(16) glm::vec3 toCol;
};