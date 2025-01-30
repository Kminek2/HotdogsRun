#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <vector>

struct MapGen {
private:
	static const glm::vec2 neighbor_map[8];

	static bool shortest_path(const glm::vec2& start, uint16_t steps);
public:
	static std::vector<glm::vec2> generateMap(uint16_t len, size_t seed = -1);

	static void spreadMapPoints(std::vector<glm::vec2>& points, float spread);
	static void offsetMapPoints(std::vector<glm::vec2>& points, float offset);
};