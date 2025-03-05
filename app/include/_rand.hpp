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
	const T& choice(const std::vector<T>& container, std::vector<float> probs = std::vector<float>());
private:
	std::mt19937 gen;
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
/// Returns a const ref. to a random element from a vector. If probs are passed, they have to add up to 1 and not contain any negatives
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
	int neg_count = 0;

	for (const auto& p : probs) {
		if (p == -1 && ++neg_count) continue; // the ++neg_count is so that i don't have to use {}
		if (p < 0) throw std::invalid_argument("neg prob lmao");
		sum += p;
	}

	if (sum > (1 + 1e-6) || (sum < (1 - 1e-6) && !neg_count)) throw std::invalid_argument("the math aint mathin");
	sum = (1 - sum) / neg_count;

	for (auto& p : probs) {
		if (p != -1) continue;
		p = sum;
		if (!(--neg_count)) break;
	}

	std::discrete_distribution<size_t> dist(probs.begin(), probs.end());
	return container[dist(gen)];
}
