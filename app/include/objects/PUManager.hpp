#pragma once

#include <concepts>

#include "objects/PowerUps/PUMaxSpeed.hpp"
#include "objects/PowerUps/PUNitro.hpp"
#include "objects/PowerUps/PUBanana.hpp"
#include "objects/PowerUps/PUBomb.hpp"

#include "_rand.hpp"
#include "MapManager.h"

class PUManager {
private:
	_rand& rand;
	MapManager* map = nullptr;

	std::vector<GameObject*> powerups;
	std::vector<float> proba;

public:
	PUManager(_rand& rand) : rand(rand) {};

	PUManager* setMapManager(MapManager* map);
	PUManager* addPowerUp(GameObject* pu, float p);
	PUManager* generatePowerUps(unsigned offset);
};
