#include "objects/PowerUps/PUBomb.hpp"

GameObject* PUBomb::bomb;

void PUBomb::OnCollide(Collisions::CollisionData* cd)
{
	collided = (cd->obj1 == gameObject
		? cd->obj2
		: cd->obj1);

	gameObject->transform->Move(glm::vec3(0, 0, -10));
	Application::Invoke([this]() {gameObject->transform->Move(glm::vec3(0, 0, 10)); }, 5000);
	
	OnActivate();
}

void PUBomb::OnActivate()
{
	collided->cm->addBombs(1);
	collided = nullptr;
}

void PUBomb::_Init() { fc = new FancyController(gameObject, .5f, .1f, 20.0f); }
void PUBomb::Update() { fc->Update(); }
void PUBomb::OnDestroy() {}