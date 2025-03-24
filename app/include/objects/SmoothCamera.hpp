#pragma once

#include "ObjectScript.h"
#include "GameObject.h"

class CinematicCamera;

class SmoothCamera : public ObjectScript {
private: 
	glm::vec3 offset;

	glm::vec3 position = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);

	float speed;

	ViewType view;

	static bool disabled;
	int cnt_after_disabled = 0;

public:
	SmoothCamera(glm::vec3 offset, float speed = .125f, ViewType view = Isometric) : offset(offset), speed(speed), view(view) {}

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	friend CinematicCamera;
};