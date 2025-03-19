#include "PowerUp.hpp"

std::set<std::string> PowerUp::car_names;

void PowerUp::Init() {
	gameObject->surface_type = NEVER_COLLIDE;

	if (car_names.size() == 0) std::cout << "[!] There are no models linked to PowerUps!\n";

	for (const std::string& car : car_names)
		Collisions::addCallback(gameObject->GetModelName(), car, [this](Collisions::CollisionData* cd) { OnCollide(cd); });

	_Init();
}