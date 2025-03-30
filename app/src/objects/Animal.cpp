#include "objects/Animal.hpp"

void Animal::refreshTarget() {
	target_pos = glm::vec2(
		rand.random(min_pos.x, max_pos.x),
		rand.random(min_pos.y, max_pos.y)
	);
}

void Animal::Init() {
	min_pos = glm::vec2(gameObject->transform->position) - range / 2.0f;
	max_pos = min_pos + glm::vec2(gameObject->transform->position);

	refreshTarget();
}

void Animal::Update() {
	if (!Time::deltaTime) return;

	glm::vec2 diff = target_pos - glm::vec2(gameObject->transform->position);

	if (glm::length(diff) <= 1.0f) {
		refreshTarget();
		return;
	}

	diff = glm::normalize(diff);

	gameObject->transform->Move(glm::vec3(diff * speed * Time::deltaTime, 0));
	gameObject->transform->RotateTo(glm::vec3(0, 0, glm::degrees(atan2(diff.y, diff.x)) + 180.0f));
}

void Animal::OnDestroy() {}