#include "objects/PUManager.hpp"

PUManager* PUManager::setMapManager(MapManager* map)
{
	this->map = map;
	return this;
}

PUManager* PUManager::addPowerUp(GameObject* pu, float p)
{
	powerups.push_back(pu);
	proba.push_back(p);

	return this;
}

PUManager* PUManager::generatePowerUps(unsigned offset)
{
	for (unsigned i = rand.random(static_cast<unsigned>(offset / 2), offset - 1); i < map->GetLen()-1; i += offset) {
		GameObject* powerup = new GameObject(*rand.choice(powerups, proba));
		powerup->transform->MoveTo(map->GetPoint(i)->transform->position + glm::vec3(0, 0, 0.3f));
		powerup->transform->RotateTo(glm::vec3(0, 0, rand.random(0.0f, 360.0f)));

		std::cout << "pu @ " << i << '\n';
	}

	return this;
}