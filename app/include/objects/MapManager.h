#pragma once
#include "ObjectScript.h"
#include "_rand.hpp"
#include "mapgen.h"

using namespace mapgen;

class MapManager :
    public ObjectScript
{
private:
	std::vector<GameObject*> points;
	std::vector<GameObject*> check_points;
	std::vector<GameObject*> mini_decors;
	std::vector<GameObject*> buildings;

	// -- road --
	const uint16_t map_len;

	// -- general --
	const size_t seed;

	// -- checkpoints --
	const unsigned cp_offset; // cp stands for checkpoint 💀

	// -- decors -- 
	const float decors_per_tile;
	const float decor_max_dist; // min_dist = MAP_TILE_SIZE

	// -- KAMIX say what that is --
	const double a;
	const double b;
	const double min_offset;
	const double max_offset;

	const float MAP_TILE_SIZE; // 127x127 mv size

	const std::vector<std::string> small_decors;

	void add_decor(_rand rand, std::vector<MapPoint> map_points);
public:
	MapManager(size_t seed = 42, float decors_per_tile = 1.5f, float decor_max_dist = 20.0f, std::vector<std::string> small_decors = {"cube"}, uint16_t map_len = 100, unsigned cp_offset = 15, float MAP_TILE_SIZE = 12.7f, double a = 20, double b = 50, double min_offset = -5, double max_offset = 5) : seed(seed), decors_per_tile(decors_per_tile), decor_max_dist(decor_max_dist), small_decors(small_decors), map_len(map_len), cp_offset(cp_offset), MAP_TILE_SIZE(MAP_TILE_SIZE), a(a), b(b), min_offset(min_offset), max_offset(max_offset) {};
    void Init() override;
	void Update() override;
	void OnDestroy() override;

	GameObject* GetPoint(unsigned index);
};

