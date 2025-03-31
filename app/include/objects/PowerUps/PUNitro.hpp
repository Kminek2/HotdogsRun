#pragma once

#include "PowerUp.hpp"

/// <summary>
/// Add nitros
/// </summary>
class PUNitro : public PowerUp {
private:
	int n;

	FancyController* fc;

public:
	PUNitro(int nitros_count = 1) : n(nitros_count) {};

	void OnCollide(Collisions::CollisionData* cd) override;
	void OnActivate() override;

	void _Init() override;
	void Update() override;
	void OnDestroy() override;

	PUNitro* copy() override { return new PUNitro(*this); }
};