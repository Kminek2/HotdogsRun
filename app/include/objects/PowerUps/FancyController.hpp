#pragma once

#include "GameObject.h"

class FancyController {
private:
	float movement_speed;
	float rotation_speed;

	float movement_amp;
	float z_min, z_max;
	bool direction = false; // 0 for down btw

	GameObject* go;
	bool initiated = false;

public:
	FancyController(GameObject* go, float movement_speed, float movement_amp, float rotation_speed) : go(go), movement_speed(movement_speed), movement_amp(movement_amp), rotation_speed(rotation_speed) {};

	void Init();
	void Update();
};