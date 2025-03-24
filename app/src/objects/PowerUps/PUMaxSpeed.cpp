#include "objects/PowerUps/PUMaxSpeed.hpp"

void PUMaxSpeed::OnCollide(Collisions::CollisionData* cd) {
	/*std::cout << "pumaxspeed\n";

	collided = (cd->obj1 == gameObject 
		? cd->obj2 
		: cd->obj1);

	gameObject->transform->Move(glm::vec3(0, 0, -10));
	Application::Invoke([this]() {gameObject->transform->Move(glm::vec3(0, 0, 10)); }, 5000);

	OnActivate();*/
}

void PUMaxSpeed::OnActivate() {
	float cv = 2;
	cv *= (1 + increase);

	SetValue(cv, 1.0f, duration);

	collided = nullptr;
}

void PUMaxSpeed::Update() {}
void PUMaxSpeed::OnDestroy() {}