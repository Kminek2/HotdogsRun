#include "objects/LockPosition.h"

LockPosition::LockPosition(Transform* transform, glm::vec3 offset) {
	this->transform = transform;
	this->offset = offset;
}

void LockPosition::Init() {}
void LockPosition::Update() {
	gameObject->transform->MoveTo(this->transform->position + offset);
}
void LockPosition::OnDestroy() {}