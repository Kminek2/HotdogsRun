#pragma once

#include <vector>
#include <array>
#include <string>

#include "GameObject.h"

class BuildingManager {
private:
private:
	const size_t seed;
	const std::array<std::vector<std::string>, 16> tiles;
	const glm::vec3 offset;
	static const float BUILDING_SIZE;

public:
	BuildingManager(size_t seed, const std::array<std::vector<std::string>, 16>& building_tiles, const glm::vec3& offset = glm::vec3(0)) : seed(seed), tiles(building_tiles), offset(offset) {};

	std::vector<GameObject*> generateBuildings(const std::vector<std::vector<bool>>& building_data);
};