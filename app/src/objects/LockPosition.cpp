#include "objects/LockPosition.h"

LockPosition::LockPosition(Transform* transform, glm::vec3 offset) {
	this->transform = transform;
	this->offset = offset;
}

void LockPosition::Init() {}
void LockPosition::Update() {
	gameObject->transform->MoveTo(transform->getModelMatrix() * glm::vec4(offset, 1));
}
void LockPosition::OnDestroy() {}