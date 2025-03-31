#include "objects/PowerUps/PUMaxSpeed.hpp"

void PUMaxSpeed::OnCollide(Collisions::CollisionData* cd) {
	std::cout << "pumaxspeed\n";

	collided = (cd->obj1 == gameObject 
		? cd->obj2 
		: cd->obj1);

	gameObject->transform->Move(glm::vec3(0, 0, -10));
	Application::Invoke([this]() { gameObject->transform->Move(glm::vec3(0, 0, 10)); }, duration + .1f);

	OnActivate();
}

void PUMaxSpeed::OnActivate() {
	float& cv = collided->cm->_maxSpeed;
	cv *= (1 + increase);

	collected = true;
	who_collected = collided;
	timer = duration/1000.0f;

	collided = nullptr;
}

void PUMaxSpeed::_Init()
{
	fc = new FancyController(gameObject, .5f, .1f, 20.0f);
}

void PUMaxSpeed::Update() {
	fc->Update();

	if (collected) {
		if (timer == 0.0f) {
			who_collected->cm->_maxSpeed = 1.0f;
			collected = false;
		}
		timer -= Time::deltaTime;
		timer = std::max(0.0f, timer);
	}
}
void PUMaxSpeed::OnDestroy() {}