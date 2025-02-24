#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <vector>
#include <set>
#include <array>

#include "mapgen_road.hpp"

namespace std {
	template<>
	struct less<glm::vec2> {
		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		}
	};

	template<typename T>
	struct less<std::pair<T, T>> {
		bool operator()(const std::pair<T, T>& a, const std::pair<T, T>& b) const {
			return std::tie(a.first, a.second) < std::tie(b.first, b.second);
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

	struct MapPoint {
		glm::vec2 pos;
		direction in, out;

		MapPoint(glm::vec2 v) : pos(v), in(direction::N), out(direction::N) {};
	};

	const std::array<glm::vec2, 8> neighbor_map = { {
		{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
	} };

	std::vector<MapPoint> generateMap(uint16_t len, const Ellipse& ellipse_data, size_t seed = -1);

	void spreadMapPoints(std::vector<glm::vec2>& points, float spread);
	void offsetMapPoints(std::vector<glm::vec2>& points, float offset);

	void spreadMapPoints(std::vector<MapPoint>& points, float spread);
	void offsetMapPoints(std::vector<MapPoint>& points, float offset);
}