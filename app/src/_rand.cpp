#include "_rand.hpp"

bool _rand::coin_toss(double weight) {
	std::bernoulli_distribution dist(weight);
	return dist(gen);
}

size_t _rand::getSeed() {
	return seed;
}
