#include "objects/PowerUps/PUBanana.hpp"

void PUBanana::OnCollide(Collisions::CollisionData* cd)
{
	if (collided) return;

	collided = (cd->obj1 == gameObject ? cd->obj2 : cd->obj1);
	gameObject->transform->Move(glm::vec3(0, 0, -10));

	OnActivate();
}

void PUBanana::OnActivate()
{
	std::cout << "banana\n";
	collided->cm->__gripMult = 1;
	timer = duration / 1000.0f;
	collected = true;
}

void PUBanana::Update()
{
	if (!collected || !collided) return;

	timer -= Time::deltaTime;
	if (timer > 0) return;

	collided->cm->__gripMult = 0.5f;
	collided = nullptr;
}

void PUBanana::OnDestroy() {}