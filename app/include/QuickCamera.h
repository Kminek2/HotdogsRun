#pragma once

#include <array>
#include <cstdint>
#include "Camera.h"
#include "Input.h"
#include "AppTime.h"

using __keybinds = std::array<uint16_t, 7>;

class QuickCamera {

public:
	QuickCamera(float srx = 1, float sry = 1, float sm = 1, __keybinds mappings = __keybinds({
		GLFW_KEY_W, GLFW_KEY_S,
		GLFW_KEY_A, GLFW_KEY_D,
		GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT,
		GLFW_KEY_LEFT_CONTROL
	})) : srx(srx), sry(sry), sm(sm), mappings(mappings) {};

	/// <summary>
	/// Handle camera's rotation, with sensitivity of (srx, sry)
	/// </summary>
	void HandleRotate();

	/// <summary>
	/// Handle camera's movement, with speed of (sm) and key binds of (mappings)
	/// </summary>
	void HandleMove();

	/// <summary>
	/// getter/setter for (srx) (no param/param)
	/// </summary>
	float _srx(float srx = -1);
	/// <summary>
	/// getter/setter for (sry) (no param/param)
	/// </summary>
	float _sry(float sry = -1);
	/// <summary>
	/// getter/setter for (sm) (no param/param)
	/// </summary>
	float _sm(float sm = -1);
	/// <summary>
	/// getter/setter for (srx = sry = sm) (no param/param). returns (srx == sry)
	/// </summary>
	bool  _sr(float sr = -1);

	/// <summary>
	/// getter/setter for (mappings) (no param/param)
	/// </summary>
	__keybinds _mappings(__keybinds mappings = __keybinds({ 0,0,0,0,0,0,0 }));

private:
	__keybinds mappings;
	float srx, sry, sm;
};
