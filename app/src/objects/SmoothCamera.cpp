#include "objects/SmoothCamera.hpp"
#include "Transform.h"

bool SmoothCamera::disabled = false;
bool SmoothCamera::disabled2 = false;

void SmoothCamera::Init() {
	Camera::main->view = view;

	position = gameObject->transform->position;
	rotation = gameObject->transform->rotation;

	disabled = false;
	disabled2 = false;
}

void SmoothCamera::Update() {
	if (!dynamic_fov)
		return;

	std::tuple<float, float, float> speeds = gameObject->cm->getSpeeds();
	Camera::main->fov = glm::mix(Camera::main->fov, min_fov + (std::get<0>(speeds) - std::get<1>(speeds)) * (max_fov - min_fov) / (std::get<2>(speeds) - std::get<1>(speeds)), .125f);
}

void SmoothCamera::LateUpdate() {
	if (disabled || disabled2) {
		cnt_after_disabled = 0;
		return;
	}
	
	++cnt_after_disabled;
	if (cnt_after_disabled < 3)
		return;

    glm::vec3 targetPosition = gameObject->transform->position;
    glm::vec3 targetRotation = gameObject->transform->rotation;

	glm::vec3 newPos = gameObject->transform->getModelMatrix() * glm::vec4(offset, 1);
	glm::vec2 newRot = glm::vec2(glm::normalize(targetRotation.z + 180.0f, 360.0f), targetRotation.y);

	glm::vec3 dir = newPos - Camera::main->cameraTransform->position;
	glm::vec2 rotDir = newRot - Camera::main->cameraTransform->rotation;
	rotDir.x = glm::normalize(rotDir.x+360.0f, 360.0f);

	if (rotDir.x > 180.0f)
		rotDir.x = -360.0f+rotDir.x;
	if (rotDir.x < -180.0f)
		rotDir.x = 360.0f+rotDir.x;

	//STYLE 1 - lazy but rotating
    //Camera::main->cameraTransform->MoveTo(glm::mix(Camera::main->cameraTransform->position, newPos, speed * Time::deltaTime));
	//Camera::main->cameraTransform->RotateTo({ targetRotation.z +  180, targetRotation.y });

	//STYLE 2 - close and rotating
	//Camera::main->cameraTransform->MoveTo(glm::mix(Camera::main->cameraTransform->position, newPos, speed * (newPos - Camera::main->cameraTransform->position).length() * Time::deltaTime));
	//Camera::main->cameraTransform->RotateTo({ targetRotation.z + 180, targetRotation.y });

	//STYLE 3 - laggy
	//Camera::main->cameraTransform->MoveTo(glm::mix(Camera::main->cameraTransform->position, newPos, speed * ((newPos - Camera::main->cameraTransform->position).length() / 1.0f) * Time::deltaTime));
	//Camera::main->cameraTransform->RotateTo({ targetRotation.z + 180, targetRotation.y });

	//STYLE 4 - smoth rot
	Camera::main->cameraTransform->MoveTo(glm::mix(Camera::main->cameraTransform->position, newPos, std::min(speed * Time::deltaTime, 1.0f)));
	Camera::main->cameraTransform->RotateTo(glm::mix(Camera::main->cameraTransform->rotation, Camera::main->cameraTransform->rotation + rotDir, std::min(speed * Time::deltaTime, 1.0f)));	
}

void SmoothCamera::OnDestroy() {
	Camera::main->view = Perspective;
}

SmoothCamera* SmoothCamera::SetDynamicFov(bool on, float min, float max)
{
	dynamic_fov = on;
	min_fov = min;
	max_fov = max;
	return this;
}
