#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"

#include <set>

class PowerUp : public ObjectScript {
public:
	virtual void OnCollide(Collisions::CollisionData* cd) = 0;
	virtual void OnActivate() = 0;

	void Init() override;
	virtual void _Init() {};

	static std::set<std::string> car_names;
};