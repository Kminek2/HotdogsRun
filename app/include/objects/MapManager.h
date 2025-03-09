#pragma once

#include "ObjectScript.h"
#include "_rand.hpp"
#include "mapgen.h"

using namespace mapgen;

class MapManager
{
private:
	std::vector<GameObject*> points; // the final map tiles
	std::vector<GameObject*> check_points;
	std::vector<GameObject*> mini_decors;
	std::vector<GameObject*> buildings;

	// -- general --
	const size_t seed;

	// -- road --
	const uint16_t map_len;
	const Ellipse ellipse;
	const float MAP_TILE_SCALE = 2.0;

	// -- checkpoints --
	const unsigned cp_offset; // cp stands for checkpoint 💀

	// -- decors -- 
	const float decors_per_tile;
	const float MAP_TILE_SIZE = 12.7; // 127x127 mv size
	const float decor_max_dist; // min_dist = MAP_TILE_SIZE

	const std::pair<std::vector<std::string>, std::vector<float>> small_decors;
	const std::pair<std::vector<std::string>, std::vector<float>> road_types;

	int num_sur_changes = 0;

	void add_decor(_rand& rand, const std::vector<MapPoint>& map_points);

public:
	struct MapSettingsValues {
		MapSettingsValues();
		float decors_per_tile = 1.5f,
			decor_max_dist = 10.0f;

		Ellipse ellipse = { 20, 50, -5, 5 };

		std::pair<std::vector<std::string>, std::vector<float>> small_decors = {};
		std::pair<std::vector<std::string>, std::vector<float>> road_types = { {"Asfalt"}, {1} };

		int num_sur_changes = 0;

		uint16_t map_len = 100;
		unsigned checkpoint_offset = 15;
	};

	// i hate this
	MapManager(size_t seed = 42, const MapSettingsValues& vals = MapSettingsValues()) :
		seed(seed),
		decors_per_tile(vals.decors_per_tile), 
		decor_max_dist(vals.decor_max_dist), 
		small_decors(vals.small_decors),
		road_types(vals.road_types),
		num_sur_changes(vals.num_sur_changes),
		map_len(vals.map_len), 
		cp_offset(vals.checkpoint_offset), 
		ellipse(vals.ellipse) {};

	MapManager* Init();
	GameObject* GetPoint(unsigned long long index);
	GameObject* GetCheckPoint(unsigned long long index);

	int GetLen();
	int GetCheckPoints();
	float GetMapTileSize();
	float GetMapScale();
};

