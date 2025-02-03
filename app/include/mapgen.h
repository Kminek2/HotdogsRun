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

struct Ellipse {
	double a;
	double b;

	double min_offset;
	double max_offset;

	Ellipse(double _a, double _b, double _mio, double _mao) : a(_a), b(_b), min_offset(_mio), max_offset(_mao) {};
};

struct MapGen {
public:
	static const std::array<glm::vec2, 8> neighbor_map;

	static std::vector<glm::vec2> generateMap(uint16_t len, const Ellipse& ellipse_data, size_t seed = -1);

	static void spreadMapPoints(std::vector<glm::vec2>& points, float spread);
	static void offsetMapPoints(std::vector<glm::vec2>& points, float offset);
	static std::vector<glm::vec2> Bresenham(glm::vec2 start, glm::vec2 end);
};
