#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"
#include "objects/CarMovement.h"
#include "objects/RaceManager.hpp"

class PowerUp : public ObjectScript {
protected:
	GameObject* collided = nullptr;

public:
	virtual void OnCollide(Collisions::CollisionData* cd) = 0;
	virtual void OnActivate() = 0;

	void Init() override;
	virtual void _Init() {};

	static std::vector<RaceManager::CarObject*> car_objects;
};