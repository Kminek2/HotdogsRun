#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <vector>
#include <set>
#include <array>

namespace std {
	template<>
	struct less<glm::vec2> {
		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		}
	};
}

struct MapGen {
private:
	static const std::array<glm::vec2, 8> neighbor_map;

	static bool shortest_path(const glm::vec2& start, const std::set<glm::vec2, std::less<glm::vec2>>& taken, uint16_t steps);
public:
	static std::vector<glm::vec2> generateMap(uint16_t len, size_t seed = -1);

	static void spreadMapPoints(std::vector<glm::vec2>& points, float spread);
	static void offsetMapPoints(std::vector<glm::vec2>& points, float offset);
};
