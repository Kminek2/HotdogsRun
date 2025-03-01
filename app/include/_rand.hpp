#pragma once

#include <random>
#include <vector>
#include <cstdint>
#include <stdexcept>

class _rand {
public:
	_rand() : gen(std::random_device{}()) {};
	explicit _rand(size_t _seed) : gen(_seed) {};

	bool coin_toss(double weight = 0.5);

	template<typename T>
	T random(T min, T max);

	template<typename T>
	const T& choice(const std::vector<T>& container);
private:
	std::mt19937 gen;
};

/// <summary>
/// [min, max] for integers, [min, max) for real
/// </summary>
template<typename T>
T _rand::random(T min, T max) {
	static_assert(std::is_arithmetic<T>::value, "bro randomizing letters or sum idk");

	if (min > max) std::swap(min, max);

	if constexpr (std::is_integral<T>::value) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(gen);
	}

	std::uniform_real_distribution<T> dist(min, max);
	return dist(gen);
}

/// <summary>
/// Returns a const ref. to a random element from a vector
/// </summary>
template<typename T>
const T& _rand::choice(const std::vector<T>& container) {
	if (container.empty()) throw std::invalid_argument("container as empty as my social life 😭");

	std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
	return container[dist(gen)];
}