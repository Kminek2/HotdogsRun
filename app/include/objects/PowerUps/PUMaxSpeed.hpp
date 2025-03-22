#pragma once

#include "PowerUp.hpp"

/// <summary>
/// Temporarly increase the max speed
/// </summary>
class PUMaxSpeed : public PowerUp {
private:
	float increase;
	unsigned duration;

public:
	PUMaxSpeed(float increase_percent, unsigned duration_ms) : increase(increase_percent), duration(duration_ms) {};

	void OnCollide(Collisions::CollisionData* cd) override;
	void OnActivate() override;
};