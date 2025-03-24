#include "objects/SmoothCamera.hpp"

void SmoothCamera::Init() {
	Camera::main->view = view;

	position = gameObject->transform->position;
	rotation = gameObject->transform->rotation;
}

void SmoothCamera::Update() {}

void SmoothCamera::LateUpdate() {
    glm::vec3 targetPosition = gameObject->transform->position;
    glm::vec3 targetRotation = gameObject->transform->rotation;

    Camera::main->cameraTransform->MoveTo(position);
    Camera::main->cameraTransform->RotateTo(rotation);
}

void SmoothCamera::OnDestroy() {
	Camera::main->view = Perspective;
}