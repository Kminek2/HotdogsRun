#include "objects/PowerUps/PUMaxSpeed.hpp"

void PUMaxSpeed::OnCollide(Collisions::CollisionData* cd) {
	collided = (cd->obj1 == gameObject 
		? cd->obj2 
		: cd->obj1);

	OnActivate();
}

void PUMaxSpeed::OnActivate() {
	float& cv = collided->cm->_maxSpeed;
	cv *= (1 + increase);

	SetValue(cv, -1.0f, duration);

	collided = nullptr;
}

void PUMaxSpeed::Update() {}
void PUMaxSpeed::OnDestroy() {}