#include "objects/LockToAxis.h"

LockToAxis::LockToAxis(GameObject* obj, glm::vec3 axis) {
	if(glm::elements(axis) != 1)
		throw std::invalid_argument("Axis must be a unit vector");

	this->axis = glm::normalize(axis);
	this->obj = obj;
}

void LockToAxis::Init() {}
void LockToAxis::Update() {
	gameObject->transform->MoveTo(obj->transform->position);
	gameObject->transform->RotateTo(glm::cross(obj->transform->rotation, axis));
}
void LockToAxis::OnDestroy() {}