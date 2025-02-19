#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <vector>
#include <set>
#include <array>
#include <map>
#include "GameObject.h"

namespace std {
	template<>
	struct less<glm::vec2> {
		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		}
	};
}

namespace mapgen {
	struct Ellipse {
		double a;
		double b;

		double min_offset;
		double max_offset;

		Ellipse(double _a, double _b, double _mio, double _mao) : a(_a), b(_b), min_offset(_mio), max_offset(_mao) {};
	};

	enum direction {
		N, NE, E, SE, S, SW, W, NW
	};

	struct MapPoint {
		glm::vec2 pos;
		mapgen::direction in, out;

		MapPoint(glm::vec2 v) : pos(v), in(mapgen::direction::N), out(mapgen::direction::N) {};
	};
}

using mapgen::direction;

struct MapGen {
public:
	static const std::array<glm::vec2, 8> neighbor_map;
	static std::map<std::pair<direction, direction>, GameObject*> road_segements;

	static std::vector<mapgen::MapPoint> generateMap(uint16_t len, const mapgen::Ellipse& ellipse_data, size_t seed = -1);

	static void spreadMapPoints(std::vector<glm::vec2>& points, float spread);
	static void offsetMapPoints(std::vector<glm::vec2>& points, float offset);

	static void spreadMapPoints(std::vector<mapgen::MapPoint>& points, float spread);
	static void offsetMapPoints(std::vector<mapgen::MapPoint>& points, float offset);
};