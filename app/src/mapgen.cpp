#include "mapgen.h"

#include <random>
#include <algorithm>
#include <queue>

const glm::vec2 MapGen::neighbor_map[8] = {
	{-1, 1}, {0, 1}, {1, 1},
	{-1, 0},		 {1, 0},
	{-1,-1}, {0,-1}, {1,-1}
};

bool MapGen::shortest_path(const glm::vec2& start, std::set<glm::vec2>& taken, uint16_t steps) {
	std::queue<std::pair<glm::vec2, uint16_t>> q;

	glm::vec2 v = start;
	q.emplace(v, steps);

	while (!q.empty()) {
		std::tie(v, steps) = q.front();

		if (steps == 0) continue;
		if (v == glm::vec2({ 0,0 })) return true;

		for (uint8_t i = 0; i < 8; i++)
			q.emplace(v + neighbor_map[i], steps - 1);
	}

	return false;
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

		if (points_set.find(pot) != points_set.end() || !shortest_path(pot, points_set, left))
			continue;

		points.push_back(pot);
		points_set.insert(pot);

		--left;
	}

	return points;
	return {};
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a*spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}