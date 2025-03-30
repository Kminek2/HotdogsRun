#pragma once

#include "GameObject.h"
#include "PowerUp.hpp"

class PUBanana : public PowerUp {
public:
	PUBanana(unsigned duration_ms) : duration(duration_ms) {};

	void OnCollide(Collisions::CollisionData* cd) override;
	void OnActivate() override;

	void Update() override;
	void OnDestroy() override;


	PUBanana* copy() override { return new PUBanana(*this); }

private:
	unsigned duration;
	bool collected = false;
	GameObject* collided = nullptr;
	double timer = 0;
};