#include "objects/LockPosition.h"

LockPosition::LockPosition(Transform* transform, glm::vec3 offset) {
	this->transform = transform;
	this->offset = offset;
}

void LockPosition::Init() {}
void LockPosition::EarlyUpdate() {}
void LockPosition::Update() {}
void LockPosition::LateUpdate() {
	gameObject->transform->MoveTo(this->transform->position + offset);
}
void LockPosition::OnDestroy() {}