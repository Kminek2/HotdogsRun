#pragma once

#include "GameObject.h"
#include "PowerUp.hpp"

/// <summary>
/// Temporarly increase the max speed
/// </summary>
class PUMaxSpeed : public PowerUp {
private:
	float increase;
	unsigned duration;
	bool collected = false;
	float timer;
	GameObject *who_collected;

public:
	PUMaxSpeed(float increase_percent, unsigned duration_ms) : increase(increase_percent), duration(duration_ms) {};

	void OnCollide(Collisions::CollisionData* cd) override;
	void OnActivate() override;

	void Update() override;
	void OnDestroy() override;

	PUMaxSpeed* copy() override { return new PUMaxSpeed(*this); }
};