#pragma once

#include <map>
#include "GameObject.h"

namespace mapgen {
	enum direction { N, NE, E, SE, S, SW, W, NW };

using __road = std::map<std::pair<mapgen::direction, mapgen::direction>, GameObject*>;

	__road createRoadMap(std::string type = "Asfalt");
}