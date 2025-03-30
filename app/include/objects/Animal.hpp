#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "_rand.hpp"

class Animal : public ObjectScript {
public:
	Animal(_rand& rand, glm::vec2 range = glm::vec2(100), float speed = 1.0f) : speed(speed), rand(rand), range(range) {};

	void Init() override;
	void Update() override;
	void OnDestroy() override;
	ObjectScript* copy() override { return new Animal(*this); };

	void refreshTarget();

private:
	glm::vec2 range;
	float speed;
	_rand& rand;

	glm::vec2 min_pos, max_pos;
	glm::vec2 target_pos;
};