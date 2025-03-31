#pragma once

#include "GameObject.h"
#include "PowerUp.hpp"
#include "objects/PowerUps/Bomba.hpp"

class PUBomb : public PowerUp {
public:
	void OnCollide(Collisions::CollisionData* cd) override;
	void OnActivate() override;

	void _Init() override;
	void Update() override;
	void OnDestroy() override;

	PUBomb* copy() override { return new PUBomb(*this); }

	static void SetBomb(GameObject* _bomb) { bomb = _bomb; }
	static GameObject* bomb;

private:
	FancyController* fc;
};