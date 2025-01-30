#include "mapgen.h"

#include <random>
#include <set>
#include <algorithm>

const glm::vec2 MapGen::neighbor_map[8] = {
	{-1, 1}, {0, 1}, {1, 1},
	{-1, 0},		 {1, 0},
	{-1,-1}, {0,-1}, {1,-1}
};

bool MapGen::shortest_path(const glm::vec2& start, uint16_t steps) {
	// bfs

	return true;
}

std::vector<glm::vec2> MapGen::generateMap(uint16_t len, size_t seed) {
	assert(len >= 3);

	if (seed == -1)
		return MapGen::generateMap(len, std::random_device()());

	std::vector<glm::vec2> points;
	points.reserve(len);

	std::set<glm::vec2> points_set;

	std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, 7); // 8 neighbors

	// always start with a straight
	points.push_back({ 0,0 }); points_set.insert({ 0,0 });
	points.push_back({ 1,0 }); points_set.insert({ 1,0 });

	glm::vec2 cur = { 1,0 };
	uint16_t left = len - 2;

	while(left) {
		const glm::vec2& pot = cur + neighbor_map[dis(gen)];

		if (points_set.find(pot) != points_set.end() || !shortest_path(pot, left))
			continue;

		points.push_back(pot);
		points_set.insert(pot);

		--left;
	}

	return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&](glm::vec2& a) { a *= spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&](glm::vec2& a) { a += offset; });
}