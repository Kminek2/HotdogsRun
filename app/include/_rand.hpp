#pragma once

#include <random>
#include <vector>
#include <cstdint>

class _rand {
public:
	_rand() : gen(std::random_device{}()) {};
	explicit _rand(size_t _seed) : gen(_seed) {};

	template<typename T>
	T random(T min, T max);

	template<typename T>
	const T& choice(const std::vector<T>& container);
private:
	std::mt19937 gen;
};