#pragma once

#include <random>
#include <cstdint>

class rand {
public:
	rand(size_t _seed = -1) : seed(_seed == -1 ? rd() : _seed) {  };

private:
	std::random_device rd;
	std::mt19937 gen;
	const size_t seed;
};