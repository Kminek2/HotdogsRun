#include "objects/PowerUps/PUNitro.hpp"

void PUNitro::OnCollide(Collisions::CollisionData* cd) {
	collided = (cd->obj1 == gameObject
		? cd->obj2
		: cd->obj1);

	OnActivate();
}

void PUNitro::OnActivate() {
	collided->cm->addNitros(n);
	collided = nullptr;
}

void PUNitro::Update() {}
void PUNitro::OnDestroy() {}