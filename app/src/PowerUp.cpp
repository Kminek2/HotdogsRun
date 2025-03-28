#include "PowerUp.hpp"

std::vector<RaceManager::CarObject*> PowerUp::car_objects;

void PowerUp::Init() {
	if (!gameObject->GetOBBsCount()) gameObject->AddDefaultOBB();
	gameObject->surface_type = NEVER_COLLIDE;

	if (car_objects.size() == 0) std::cout << "[!] There are no models linked to PowerUps!\n";

	for (const auto& car : car_objects) {
		Collisions::addCallback(gameObject, car->car, [this](Collisions::CollisionData* cd) { if(cd->obj1 == gameObject || cd->obj2 == gameObject) OnCollide(cd); });
	}

	_Init();
}