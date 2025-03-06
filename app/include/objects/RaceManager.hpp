#pragma once

#include "MapManager.h"
#include "GameObject.h"

#include <glm/vec2.hpp>
#include <array>

class RaceManager {
private:
	MapManager* map_manager = nullptr;
	int cars_placed = 0;

	const static std::array<glm::vec2, 4> offsets;

public:
	RaceManager* SetMapManager(MapManager* map_manager);
	RaceManager* AddCar(GameObject* car);
};