#include "objects/PowerUps/PUNitro.hpp"

void PUNitro::OnCollide(Collisions::CollisionData* cd) {
	std::cout << "punitro\n";

	collided = (cd->obj1 == gameObject
		? cd->obj2
		: cd->obj1);

	gameObject->transform->Move(glm::vec3(0, 0, -10));
	Application::Invoke([this]() {gameObject->transform->Move(glm::vec3(0, 0, 10)); }, 5000);

	OnActivate();
}

void PUNitro::OnActivate() {
	collided->cm->addNitros(n);
	collided = nullptr;
}

void PUNitro::Update() {}
void PUNitro::OnDestroy() {}