#include "objects/BuildingManager.hpp"
#include "_rand.hpp"

const float BuildingManager::BUILDING_SIZE = 6.4;
const std::array<std::array<uint8_t, 2>, 4> nmap = { {{0,1}, {1,1}, {1,0}, {0,0} } };

std::vector<GameObject*> BuildingManager::generateBuildings(const std::vector<std::vector<bool>>& building_data) {
	std::vector<GameObject*> buildings;

	// -- assert -- 
	const size_t _w = building_data.size();
	if (_w == 0) throw std::invalid_argument("empty");
	for (const auto& row : building_data) if (row.size() != _w) throw std::invalid_argument("not square");

	buildings.reserve((_w - 1) * (_w - 1));

	_rand rand(seed);

	char d;
	for (size_t y = 0; y < _w-1; y++)
		for (size_t x = 0; x < _w-1; x++) {
			d = 0;

			for (int i = 0; i < 4; i++)
				if (building_data[y + nmap[i][1]][x + nmap[i][0]])
					d |= 1 << i;

			if (!tiles[d].empty())
				buildings.push_back(new GameObject(rand.choice(tiles[d]), offset + glm::vec3(x, y, 0) * BUILDING_SIZE));
		}

	return buildings;
}