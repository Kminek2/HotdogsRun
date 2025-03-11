#include "objects/BuildingManager.hpp"
#include "Collisions.h"
#include "_rand.hpp"

const float BuildingManager::BUILDING_SIZE = 6.4;
const std::array<std::array<uint8_t, 2>, 4> nmap = { {{0,0}, {1,0}, {1,1}, {0,1} } };

BuildingManager* BuildingManager::setOffset(glm::vec3 offset) {
	this->offset = offset;
	return this;
}

BuildingManager* BuildingManager::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
	return this;
}

BuildingManager* BuildingManager::setScale(glm::vec3 scale) {
	this->scale = scale;
	return this;
}

BuildingManager* BuildingManager::setMap(std::vector<GameObject*>* map)
{
	this->map = map;
	return this;
}

glm::vec3 BuildingManager::getOffset() { return offset; }
glm::vec3 BuildingManager::getRotation() { return rotation; }
glm::vec3 BuildingManager::getScale() { return scale; }

std::vector<GameObject*> BuildingManager::generateBuildings(const std::vector<std::vector<bool>>& building_data, bool centerAtOffset) {
	std::vector<GameObject*> buildings;

	// -- assert -- 
	const size_t _w = building_data.size();
	if (_w == 0) throw std::invalid_argument("empty");
	for (const auto& row : building_data) if (row.size() != _w) throw std::invalid_argument("not square");

	buildings.reserve((_w - 1) * (_w - 1));

	_rand rand(seed);

	const glm::vec3 correction_offset = offset - (centerAtOffset ? glm::vec3((_w - 1) / 2) * glm::vec3(1, 1, 0) : glm::vec3(0)) * BUILDING_SIZE;

	char d;
	for (size_t y = 0; y < _w-1; y++)
		for (size_t x = 0; x < _w-1; x++) {
			d = 0;

			for (int i = 0; i < 4; i++)
				if (building_data[y + nmap[i][1]][x + nmap[i][0]])
					d |= 1 << i;

			if (!tiles[d].empty()) {
				buildings.push_back(new GameObject(rand.choice(tiles[d]), correction_offset + glm::vec3(x, y, 0) * BUILDING_SIZE, rotation, scale));
				buildings[buildings.size()-1]->AddDefaultOBB();
			}
		}

	return buildings;
}

std::vector<std::vector<bool>> BuildingManager::generateBuildingsVector(unsigned int centerRoad, unsigned int citySize)
{

	_rand rand(seed);

	std::vector<std::vector<bool>> points(citySize);
	glm::vec3 posNow = glm::vec3(map->at(centerRoad)->transform->position) - glm::vec3(BUILDING_SIZE * citySize / 2);
	for (int y = 0; y < citySize; y++) {
		points[y] = std::vector<bool>(citySize);
		for (int x = 0; x < citySize; x++) {
			GameObject* building = new GameObject("case_15", posNow, glm::vec3(0), glm::vec3(.5f));
			GameObject* buildingCloser = new GameObject("case_15", posNow);
			building->AddDefaultOBB(glm::vec3(0), true);
			buildingCloser->AddDefaultOBB(glm::vec3(BUILDING_SIZE), true);
			bool canPlace = true;
			bool closeToRoad = false;
			for(GameObject* road : *map)
				if (Collisions::checkCollision(*road, *buildingCloser))
					closeToRoad = true;
			for (GameObject* road : *map) {
				//Collisions::checkCollision(building, )
				//MAKE DOING FALSE IF COOLISION IS TRUE AND CONTINUE IN HEIGHER FOR
				if (Collisions::checkCollision(*road, *building)) {
					canPlace = false;
					break;
				}
			}
			delete building;
			delete buildingCloser;
			points[y][x] = canPlace && (closeToRoad || rand.coin_toss((1 - std::abs((y - citySize / 2.0f) / (citySize / 2.0f))) * (1 - std::abs((x - citySize / 2.0f) / (citySize / 2.0f)))));

			posNow.x += BUILDING_SIZE;
		}
		posNow.y += BUILDING_SIZE;
		posNow.x -= BUILDING_SIZE * citySize;
	}

	return points;
}
