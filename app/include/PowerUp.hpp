#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "Collisions.h"
#include "objects/CarMovement.h"

#include <set>

class PowerUp : public ObjectScript {
protected:
	GameObject* collided = nullptr;

public:
	virtual void OnCollide(Collisions::CollisionData* cd) = 0;
	virtual void OnActivate() = 0;

	void Init() override;
	virtual void _Init() {};
	
	template<typename T>
	static void SetValue(T var, T val, unsigned cooldown_ms);

	static std::set<std::string> car_names;
};

template<typename T>
void PowerUp::SetValue(T var, T val, unsigned cooldown_ms) {
	Application::Invoke([&]() { var = val; }, cooldown_ms);
}