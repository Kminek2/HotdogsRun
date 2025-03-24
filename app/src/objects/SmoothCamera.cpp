#include "objects/SmoothCamera.hpp"

bool SmoothCamera::disabled = false;

void SmoothCamera::Init() {
	Camera::main->view = view;

	position = gameObject->transform->position;
	rotation = gameObject->transform->rotation;

	disabled = false;
}

void SmoothCamera::Update() {}

void SmoothCamera::LateUpdate() {
	if (disabled) {
		cnt_after_disabled = 0;
		return;
	}
	
	++cnt_after_disabled;
	if (cnt_after_disabled < 3)
		return;

    glm::vec3 targetPosition = gameObject->transform->position;
    glm::vec3 targetRotation = gameObject->transform->rotation;

	glm::vec3 newPos = gameObject->transform->getModelMatrix() * glm::vec4(offset, 1);
	glm::vec2 newRot = glm::vec2(glm::normalize(targetRotation.z + 180, 360.0f), targetRotation.y);

	glm::vec3 dir = newPos - Camera::main->cameraTransform->position;
	glm::vec2 rotDir = newRot - Camera::main->cameraTransform->rotation;
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
	Camera::main->cameraTransform->MoveTo(glm::mix(Camera::main->cameraTransform->position, newPos, speed * Time::deltaTime));
	Camera::main->cameraTransform->RotateTo(glm::mix(Camera::main->cameraTransform->rotation, newRot, speed * Time::deltaTime));

	glm::vec3 newDir = newPos - Camera::main->cameraTransform->position;
	glm::vec2 newRotDir = newRot - Camera::main->cameraTransform->rotation;

	if (dir == -newDir) {
		Camera::main->cameraTransform->MoveTo(newPos);
	}

	if (rotDir == -newRotDir) {
		Camera::main->cameraTransform->RotateTo(newRotDir);
	}
}

void SmoothCamera::OnDestroy() {
	Camera::main->view = Perspective;
}