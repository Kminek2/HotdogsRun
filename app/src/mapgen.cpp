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

/// <summary>
/// Implementation of Beresenham's algorithm. Includes `start` and `end` in the result.
/// </summary>
std::vector<glm::vec2> bresenham(const glm::ivec2& start, const glm::ivec2& end) {
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

std::vector<glm::vec2> MapGen::generateMap(uint16_t len, const Ellipse& ellipse_data, size_t seed) {
    if (seed == static_cast<size_t>(-1)) {
        std::random_device rd;
        seed = rd();
    }

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> offset_dist(ellipse_data.min_offset, ellipse_data.max_offset);

    std::vector<glm::vec2> points;

    const double Δθ = 2 * glm::pi<double>() / len;

    double Δx, Δx0;
    Δx = Δx0 = offset_dist(gen);

    for (uint16_t i = 1; i < len; i++) {
        double θ1 = Δθ * i;
        double Δx1 = Δx;

        double θ2 = θ1 + Δθ;
        double Δx2 = offset_dist(gen);

        std::vector<glm::vec2> line(bresenham(
            { (ellipse_data.a + Δx1) * std::cos(θ1), (ellipse_data.b + Δx1) * std::sin(θ1) },
            { (ellipse_data.a + Δx2) * std::cos(θ2), (ellipse_data.b + Δx2) * std::sin(θ2) }
        ));

        line.pop_back();
        points.insert(points.end(), line.begin(), line.end());

        Δx = Δx2;
    }

    std::vector<glm::vec2> f_line(bresenham(
        { (ellipse_data.a + Δx ) * std::cos(0 ), (ellipse_data.b + Δx ) * std::sin(0 ) },
        { (ellipse_data.a + Δx0) * std::cos(Δθ), (ellipse_data.b + Δx0) * std::sin(Δθ) }
    ));

    f_line.pop_back();
    points.insert(points.end(), f_line.begin(), f_line.end());

    return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a * spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}