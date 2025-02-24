#include "mapgen.h"

#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>
#include "glm/gtc/constants.hpp"

using mapgen::direction;

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
        if (e2 < dx)  { err += dx; y += sy; }
    }

    return result;
}

void emplace_vectors(std::vector<mapgen::MapPoint>& in, const std::vector<glm::vec2>& src) {
    for (size_t i=0; i<src.size()-1; i++)
        in.emplace_back(src[i]);
}

direction points_direction(const glm::vec2& a, const glm::vec2& b) {
    for (uint8_t i = 0; i < 8; i++)
        if ((a + mapgen::neighbor_map[i]) == b)
            return direction((i + 4) % 8); // +0 is the right offset

    throw std::invalid_argument("kys");
    return direction(0);
}

std::vector<mapgen::MapPoint> mapgen::generateMap(uint16_t len, const mapgen::Ellipse& ellipse_data, size_t seed) {
    if (seed == static_cast<size_t>(-1)) {
        std::random_device rd;
        seed = rd();
    }

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> offset_dist(ellipse_data.min_offset, ellipse_data.max_offset);

    std::vector<mapgen::MapPoint> points;

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

        emplace_vectors(points, line);

        Δx = Δx2;
    }

    std::vector<glm::vec2> f_line(bresenham(
        { (ellipse_data.a + Δx ) * std::cos(0 ), (ellipse_data.b + Δx ) * std::sin(0 ) },
        { (ellipse_data.a + Δx0) * std::cos(Δθ), (ellipse_data.b + Δx0) * std::sin(Δθ) }
    ));

    emplace_vectors(points, f_line);

    // -- in --
    for (size_t i = 1; i < points.size(); i++)
        points[i].in = points_direction(points[i-1].pos, points[i].pos);
    points[0].in = points_direction(points[points.size() - 1].pos, points[0].pos);

    // -- out --
    for (size_t i = 0; i < points.size() - 1; i++)
        points[i].out = points_direction(points[i].pos, points[i+1].pos);
    points[points.size() - 1].out = points_direction(points[0].pos, points[points.size() - 1].pos);

    return points;
}

void mapgen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a * spread; });
}
void mapgen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}

void mapgen::spreadMapPoints(std::vector<mapgen::MapPoint>& points, float spread) {
    for (auto& point : points)
        point.pos *= spread;
}
void mapgen::offsetMapPoints(std::vector<mapgen::MapPoint>& points, float offset) {
    for (auto& point : points)
        point.pos += glm::vec2(offset);
}