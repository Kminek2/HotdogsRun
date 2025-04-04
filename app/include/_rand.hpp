﻿#pragma once

#include <random>
#include <vector>
#include <cstdint>
#include <stdexcept>

class _rand {
public:
	_rand() : seed(std::random_device{}()) { gen = std::mt19937(seed); };
	explicit _rand(size_t _seed) : seed(_seed), gen(_seed) {};

	bool coin_toss(double weight = 0.5);

	template<typename T>
	T random(T min, T max);

	template<typename T>
	const T& choice(const std::vector<T>& container, std::vector<float> probs = std::vector<float>());

	size_t getSeed();
private:
	std::mt19937 gen;
	size_t seed;
};

/// <summary>
/// Returns random value from the range [min, max] for integers and [min, max) for floating-point values.
/// </summary>
template<typename T>
T _rand::random(T min, T max) {
	static_assert(std::is_arithmetic_v<T>, "bro randomizing letters or sum 😭");

	if (min > max) std::swap(min, max);

	if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(gen);
	}
	else {
		std::uniform_real_distribution<T> dist(min, max);
		return dist(gen);
	}
}

/// <summary>
/// Returns a const ref. to a random element from a vector
/// </summary>
template<typename T>
const T& _rand::choice(const std::vector<T>& container, std::vector<float> probs) {
	if (container.empty()) throw std::invalid_argument("container as empty as my social life 😭");

	if (probs.empty()) {
		std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
		return container[dist(gen)];
	}

	if(probs.size() > container.size()) probs.resize(container.size());

	float sum = 0;
	float neg_sum = 0;

	for (const auto& p : probs) {
		if (p < 0) neg_sum -= p;
		else sum += p;
	}

	sum = 1 - sum; // this how much to be divided
	neg_sum = sum / -neg_sum; // this how much (-1) is worth

	for (auto& p : probs) {
		if (p >= 0) continue;
		p = std::max(0.0f, p * neg_sum);
	}

	std::discrete_distribution<size_t> dist(probs.begin(), probs.end());
	return container[dist(gen)];
}
