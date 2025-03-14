#include "objects/CameraLockScript.h"

bool CameraLockScript::disabled = false;

CameraLockScript::CameraLockScript(ViewType view, glm::vec3 offset, float pitch, float yaw, bool lock_rotation, bool unlocked_rotation, uint16_t key_left, uint16_t key_right)
: lock_rotation(lock_rotation), unlocked_rotation(unlocked_rotation), view(view), key_left(key_left), key_right(key_right) {
	this->offset = offset;
	this->pitch = pitch;
	this->yaw = yaw;
}

void CameraLockScript::Init() {
	Camera::main->view = view;
	disabled = false;
}
void CameraLockScript::Update() {}

void CameraLockScript::LateUpdate() {
	if (disabled) {
		cnt_after_disabled = 0;
		return;
	}
	++cnt_after_disabled;
	if (cnt_after_disabled < 3)
		return;
		
	if (lock_rotation) {
		yaw = gameObject->transform->rotation.z+180.0f;
	} else if (unlocked_rotation) {
		if (Input::getKeyPressed(key_right)) {
			yaw += 20*Time::deltaTime;
			while (yaw > 360)
				yaw -= 360;
		}
		if (Input::getKeyPressed(key_left)) {
			yaw -= 20*Time::deltaTime;
			while (yaw < 0)
				yaw += 360;
		}
	}
	Camera::main->cameraTransform->MoveTo(gameObject->transform->position);
	Camera::main->cameraTransform->RotateTo(yaw, pitch);
	Camera::main->cameraTransform->Translate(offset);
}

void CameraLockScript::OnDestroy() {
	Camera::main->view = Perspective;
}