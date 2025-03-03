#pragma once

#include <vector>
#include <array>
#include <string>

#include "GameObject.h"

class BuildingManager {
private:
	const size_t seed;
	const std::array<std::vector<std::string>, 16> tiles;

public:
	BuildingManager(size_t seed, const std::array<std::vector<std::string>, 16>& building_tiles) : seed(seed), tiles(building_tiles) {};

	std::vector<GameObject*> generateBuildings(const std::vector<std::vector<bool>>& building_data);
};