#pragma once

#include <vector>
#include <array>
#include <string>

#include "GameObject.h"
#include "_rand.hpp"

class BuildingManager {
private:
private:
	_rand rand;
	const std::array<std::vector<std::string>, 16> tiles;

	glm::vec3 offset = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	static const float BUILDING_SIZE;

	std::vector<GameObject*>* map = nullptr;
	std::vector<GameObject*> city_roads;

public:
	BuildingManager(size_t seed, const std::array<std::vector<std::string>, 16>& building_tiles) : rand(seed), tiles(building_tiles) {};

	BuildingManager* setOffset(glm::vec3 offset);
	BuildingManager* setRotation(glm::vec3 rotation);
	BuildingManager* setScale(glm::vec3 scale);
	BuildingManager* setMap(std::vector<GameObject*>*);

	glm::vec3 getOffset();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	std::vector<GameObject*> generateBuildings(const std::vector<std::vector<bool>>& building_data);
	std::vector<std::vector<bool>> generateBuildingsVector(unsigned int centerRoad, unsigned int citySize);

	BuildingManager* replaceCityRoads(std::map<std::string, std::pair<std::vector<std::string>, std::vector<float>>> types, _rand& rand);
};