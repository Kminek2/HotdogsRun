#include "objects/PowerUps/FancyController.hpp"

void FancyController::Init()
{
	initiated = true;

	z_min = go->transform->position.z - movement_amp;
	z_max = z_min + 2 * movement_amp;
}

void FancyController::Update()
{
	if (!initiated) Init();

	go->transform->Move(glm::vec3(0, 0, 1) * Time::deltaTime * movement_speed * (direction ? 1.0f : -1.0f));

	if (direction) { if (go->transform->position.z > z_max) direction = false; }
	else { if (go->transform->position.z < z_min) direction = true; }

	go->transform->Rotate(glm::vec3(0, 0, 1) * Time::deltaTime * rotation_speed);

	//std::cout << go << ": " << "z_min: " << z_min << " | z_max: " << z_max << " | z: " << go->transform->position.z << '\n';
}