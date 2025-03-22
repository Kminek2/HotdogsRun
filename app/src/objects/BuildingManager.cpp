#include "objects/BuildingManager.hpp"
#include "Collisions.h"

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

BuildingManager* BuildingManager::setCityRoads(std::map<std::string, std::pair<std::vector<std::string>, std::vector<float>>> types)
{
	this->types = types;
	return this;
}

glm::vec3 BuildingManager::getOffset() { return offset; }
glm::vec3 BuildingManager::getRotation() { return rotation; }
glm::vec3 BuildingManager::getScale() { return scale; }

BuildingManager* BuildingManager::generateCities(unsigned n) {
	unsigned int mapCityTile = rand.random(static_cast<size_t>(0), map->size() - 1);
	for (int i = 0; i < n; i++) {
		setOffset((*map)[mapCityTile]->transform->position);
		cities.push_back(generateBuildings(generateBuildingsVector(mapCityTile, 20)));
		mapCityTile = (mapCityTile + map->size() / n) % map->size();
	}

	return this;
}

std::vector<GameObject*> BuildingManager::generateBuildings(const std::vector<std::vector<bool>>& building_data) {
	std::vector<GameObject*> buildings;

	// -- assert -- 
	const size_t _w = building_data.size();
	if (_w == 0) throw std::invalid_argument("empty");
	for (const auto& row : building_data) if (row.size() != _w) throw std::invalid_argument("not square");

	buildings.reserve((_w - 1) * (_w - 1));

	const glm::vec3 correction_offset = offset - glm::vec3((_w - 1) / 2) * glm::vec3(1, 1, 0) * BUILDING_SIZE;

	char d;
	for (size_t y = 0; y < _w-1; y++)
		for (size_t x = 0; x < _w-1; x++) {
			d = 0;

			for (int i = 0; i < 4; i++)
				if (building_data[y + nmap[i][1]][x + nmap[i][0]])
					d |= 1 << i;

			if (!tiles[d].empty()) {
				buildings.push_back(new GameObject(rand.choice(tiles[d], proba), correction_offset + glm::vec3(x, y, 0) * BUILDING_SIZE, rotation, scale));
				buildings[buildings.size()-1]->AddDefaultOBB();
			}
		}

	return buildings;
}

std::vector<std::vector<bool>> BuildingManager::generateBuildingsVector(unsigned int centerRoad, unsigned int citySize)
{
	std::vector<std::vector<bool>> points(citySize);

	glm::vec3 posNow = glm::vec3(map->at(centerRoad)->transform->position) - glm::vec3(BUILDING_SIZE * citySize / 2);

	for (int y = 0; y < citySize; y++) {
		points[y].assign(citySize, false);

		for (int x = 0; x < citySize; x++) {
			GameObject* building = (new GameObject("case_15", posNow))->AddDefaultOBB(glm::vec3(4.0f), true);
			GameObject* buildingCloser = (new GameObject("case_15", posNow))->AddDefaultOBB(glm::vec3(BUILDING_SIZE), true);

			bool canPlace = true;
			bool closeToRoad = false;

			for (GameObject* road : *map) {
				if (Collisions::checkCollision(*road, *buildingCloser)) {
					closeToRoad = true;
					city_roads.push_back(road);
				}

				if (Collisions::checkCollision(*road, *building)) 
					canPlace = false;

				if(closeToRoad && !canPlace)
					break;
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

BuildingManager* BuildingManager::replaceCityRoads()
{
	for (auto& road : city_roads) {
		std::string name = road->GetModelName();
		if (types.find(name) == types.end())
			continue;

		std::pair<std::vector<std::string>, std::vector<float>> replacements = types.at(name);
		replacements.first.push_back(name);
		replacements.second.push_back(-1);

		std::string repl = rand.choice(replacements.first, replacements.second);

		if(repl != name)
			road->ChangeModel(repl);
	}

	return this;
}

std::vector<std::vector<GameObject*>>& BuildingManager::getBuildings() {
	return cities;
}