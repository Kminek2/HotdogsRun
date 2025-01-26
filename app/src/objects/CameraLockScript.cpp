#include "objects/CameraLockScript.h"

CameraLockScript::CameraLockScript(ViewType view, glm::vec3 offset, float pitch, float yaw) {
	this->view = view;
	this->offset = offset;
	this->pitch = pitch;
	this->yaw = yaw;
}

void CameraLockScript::Init() {
	Camera::main->view = view;
}

void CameraLockScript::Update() {
	Camera::main->MoveCameraTo(gameObject->transform->position + offset);
	Camera::main->RotateCameraTo(yaw, pitch);
}

void CameraLockScript::OnDestroy() {
	Camera::main->view = Perspective;
}