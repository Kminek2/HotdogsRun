#include "mapgen.h"

#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>
#include "glm/gtc/constants.hpp"

#include <iostream>

const std::array<glm::vec2, 8> MapGen::neighbor_map = { {
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0},         {1, 0},
    {-1,-1}, {0,-1}, {1,-1}
} };

std::vector<glm::vec2> bresenham(glm::vec2 start, glm::vec2 end) {
    std::vector<glm::vec2> result;

    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = (start.x < end.x) ? 1 : -1;
    int sy = (start.y < end.y) ? 1 : -1;

    int err = dx - dy;
    int x = start.x, y = start.y;

    while (true) {
        result.emplace_back(x, y);

        if (x == end.x && y == end.y) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }

    return result;
}

bool dfs(glm::vec2& start, uint16_t n, std::vector<glm::vec2>& points, std::set<glm::vec2>& taken, std::mt19937& gen) {
    if (n == 0) return true;

    std::array<glm::vec2, 8> n_map = MapGen::neighbor_map;
    std::shuffle(std::begin(n_map), std::end(n_map), gen);

    points.push_back(start);
    taken.insert(start);

    for (glm::vec2 neighbor : n_map) {
        glm::vec2 cur = start + neighbor;

        if (!taken.count(cur)) 
            if (dfs(cur, n - 1, points, taken, gen))
                return true;
    }

    points.pop_back();
    taken.erase(taken.find(start));

    return false;
}

std::vector<glm::vec2> MapGen::generateMap(uint16_t len, const Ellipse& ellipse_data, size_t seed) {
    if (seed == static_cast<size_t>(-1)) {
        std::random_device rd;
        seed = rd();
    }
    bresenham(glm::vec2(0, 0), glm::vec2(3, 13));

    std::mt19937 gen(seed);

    std::vector<glm::vec2> points;
    points.reserve(len);

    std::uniform_real_distribution<double> offset_dist(ellipse_data.min_offset, ellipse_data.max_offset);

    const double theta_offset = 2 * glm::pi<double>() / len;

    for (uint16_t i = 0; i < len; i++) {
        double theta = theta_offset * i;
        double offset = offset_dist(gen);

        points.emplace_back((ellipse_data.a + offset) * std::cos(theta), (ellipse_data.b + offset) * std::sin(theta));
    }

    return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a * spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}