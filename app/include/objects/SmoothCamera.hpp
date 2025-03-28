#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "CarMovement.h"

class CinematicCamera;

class SmoothCamera : public ObjectScript {
private: 
	glm::vec3 offset;

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);

	float speed;

	ViewType view;

	int cnt_after_disabled = 0;
	
	bool dynamic_fov = false;
	float min_fov = 45.0f, max_fov = 90.0f;
	
public:
	SmoothCamera(glm::vec3 offset, float speed = 5.0f, ViewType view = Isometric) : offset(offset), speed(speed), view(view) {}
	
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;
	
	SmoothCamera* SetDynamicFov(bool on, float min = 45.0f, float max = 90.0f);
	
	static bool disabled;
	static bool disabled2;

	friend CinematicCamera;
};