#include "mapgen.h"

#include <queue>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>
#include "glm/gtc/constants.hpp"

#include <iostream>

using mapgen::direction;

const std::array<glm::vec2, 8> MapGen::neighbor_map = { {
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0},         {1, 0},
    {-1,-1}, {0,-1}, {1,-1}
} };

std::map<std::pair<direction, direction>, GameObject*> createRoadMap(std::string type = "Asfalt") {
    std::map<std::pair<direction, direction>, GameObject*> roadMap;

    // ========= STRAIGHT PIECES =========
    // Cardinal opposites use "prosta"
    roadMap[{direction::N, direction::S}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::S, direction::N}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,180,0 }, { 1,1,1 });
    roadMap[{direction::E, direction::W}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::W, direction::E}] = new GameObject("prosta" + type, { 0,0,0 }, { 0,180,0 }, { 1,1,1 });

    // Diagonal opposites use "prostySkos"
    roadMap[{direction::NE, direction::SW}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SW, direction::NE}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,180,0 }, { 1,1,1 });
    roadMap[{direction::NW, direction::SE}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::NW}] = new GameObject("prostySkos" + type, { 0,0,0 }, { 0,180,0 }, { 1,1,1 });

    // ========= 90° TURNS =========
    // Among cardinal directions:
    roadMap[{direction::N, direction::E}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::E, direction::S}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::S, direction::W}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::W, direction::N}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

    roadMap[{direction::E, direction::N}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::S, direction::E}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::W, direction::S}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::N, direction::W}] = new GameObject("zakret" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    // Among diagonal directions (90° difference):
    roadMap[{direction::NE, direction::SE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::SW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SW, direction::NW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NW, direction::NE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

    roadMap[{direction::SE, direction::NE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::SW, direction::SE}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::NW, direction::SW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });
    roadMap[{direction::NE, direction::NW}] = new GameObject("zakrestSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    // ========= 45° TURNS (Acute) =========
    // Cardinal -> Diagonal (where the turn is only 45°)
    roadMap[{direction::N, direction::NE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NE, direction::N}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::E, direction::SE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::E}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::S, direction::SW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SW, direction::S}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::W, direction::NW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NW, direction::W}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    // Also diagonal -> Cardinal acute turns (the “other way”)
    roadMap[{direction::NE, direction::E}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::E, direction::NE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::SE, direction::S}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::S, direction::SE}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::SW, direction::W}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::W, direction::SW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::NW, direction::N}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::N, direction::NW}] = new GameObject("zakretSkosOstry" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    // ========= 135° TURNS (Obtuse) =========
    // Cardinal -> Diagonal (non-adjacent; 135° turn)
    roadMap[{direction::N, direction::NW}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NW, direction::N}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::E, direction::NE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NE, direction::E}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::S, direction::SE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::S}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::W, direction::SW}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SW, direction::W}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    // Also, additional 135° turns between a cardinal and the opposite diagonal:
    roadMap[{direction::N, direction::SE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::N}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::E, direction::SW}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SW, direction::E}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::S, direction::NE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::NE, direction::S}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    roadMap[{direction::W, direction::SE}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });
    roadMap[{direction::SE, direction::W}] = new GameObject("zakretPolSkos" + type, { 0,0,0 }, { 0,180,0 }, { -1,1,1 });

    return roadMap;
}

std::map<std::pair<direction, direction>, GameObject*> MapGen::road_segements; //= createRoadMap("Asfalt");

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
        if (a + MapGen::neighbor_map[i] == b)
            return direction((i + 3) % 8); // +3 is the right offset

    return direction(0);
}

std::vector<mapgen::MapPoint> MapGen::generateMap(uint16_t len, const mapgen::Ellipse& ellipse_data, size_t seed) {
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
        points[i].in = points_direction(points[i].pos, points[i].pos);
    points[0].in = points_direction(points[points.size() - 1].pos, points[0].pos);

    // -- out --
    for (size_t i = 0; i < points.size() - 1; i++)
        points[i].out = points_direction(points[i + 1].pos, points[i].pos);
    points[points.size() - 1].out = points_direction(points[0].pos, points[points.size() - 1].pos);

    return points;
}

void MapGen::spreadMapPoints(std::vector<glm::vec2>& points, float spread) {
	std::transform(points.begin(), points.end(), points.begin(), [&spread](glm::vec2& a) { return a * spread; });
}
void MapGen::offsetMapPoints(std::vector<glm::vec2>& points, float offset) {
	std::transform(points.begin(), points.end(), points.begin(), [&offset](glm::vec2& a) { return a + glm::vec2(offset); }); 
}

void MapGen::spreadMapPoints(std::vector<mapgen::MapPoint>& points, float spread) {
    std::transform(points.begin(), points.end(), points.begin(), [&spread](mapgen::MapPoint& a) { a.pos *= spread; return a; });
}
void MapGen::offsetMapPoints(std::vector<mapgen::MapPoint>& points, float offset) {
    std::transform(points.begin(), points.end(), points.begin(), [&offset](mapgen::MapPoint& a) { a.pos += glm::vec2(offset); return a; });
}
