#include "objects/LockRotation.h"

LockRotation::LockRotation(Transform* transform, glm::vec3 offset) {
	this->transform = transform;
	this->offset = offset;
}

void LockRotation::Init() {}
void LockRotation::Update() {
	gameObject->transform->RotateTo(this->transform->rotation + offset);
}
void LockRotation::OnDestroy() {}