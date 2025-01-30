#pragma once

#include <array>
#include <cstdint>
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"

namespace qc {
	void HandleRotate(float s = 1);
	void HandleRotate(float sx, float sy);

	void HandleMove(float s = 1, std::array<uint16_t, 6> mappings = std::array<uint16_t, 6>({
		GLFW_KEY_W, GLFW_KEY_S,
		GLFW_KEY_A, GLFW_KEY_D,
		GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT
	}));
};
