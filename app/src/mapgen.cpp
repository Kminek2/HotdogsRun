#include "mapgen.h"

#include <queue>
#include <random>
#include <algorithm>

#include <iostream>

const std::array<glm::vec2, 8> MapGen::neighbor_map = { {
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0},         {1, 0},
    {-1,-1}, {0,-1}, {1,-1}
} };

bool MapGen::shortest_path(const glm::vec2& start, const std::set<glm::vec2, std::less<glm::vec2>>& taken, uint16_t steps) {
    std::queue<std::pair<glm::vec2, uint16_t>> q;
    q.emplace(start, steps);

    while (!q.empty()) {
        auto [v, steps_left] = q.front();
        q.pop();

        if (steps_left == 0) continue;

        for (const auto& dir : neighbor_map) {
            glm::vec2 neighbor = v + dir;

            if (neighbor == glm::vec2(0, 0) && steps_left == 1) return true; 
            if (taken.count(neighbor)) continue;

            q.emplace(neighbor, steps_left - 1);
        }
    }

    return false;
}

std::vector<glm::vec2> MapGen::generateMap(uint16_t len, size_t seed) {
    assert(len >= 3);

    if (seed == static_cast<size_t>(-1)) {
        std::random_device rd;
        seed = rd();
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 7);

    std::vector<glm::vec2> points = { {0, 0}, {1, 0} };
    std::set<glm::vec2> taken = { {0, 0}, {1, 0} };

    glm::vec2 current = { 1, 0 };
    uint16_t remaining = len - 2;

    while (remaining) {
        glm::vec2 neighbor = current + neighbor_map[dis(gen)];

        if (taken.count(neighbor) || !shortest_path(neighbor, taken, remaining))
            continue;

        points.push_back(neighbor);
        taken.insert(neighbor);
        current = neighbor;

        --remaining;
    }

    return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a*spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}