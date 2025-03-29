#pragma once

#include "ObjectScript.h"
#include "_rand.hpp"
#include "mapgen.h"
#include "objects/BuildingManager.hpp"

using namespace mapgen;

class MapDemo;

class MapManager
{
private:
	std::vector<GameObject*> points; // the final map tiles
	std::vector<GameObject*> check_points;
	std::vector<GameObject*> decors;

	// -- general --
	_rand rand;
	const size_t seed;

	// -- road --
	const uint16_t map_len;
	const Ellipse ellipse;
	const float MAP_TILE_SCALE = 2.4;

	// -- decors -- 
	const float decors_per_tile;
	const float MAP_TILE_SIZE = 12.7; // 127x127 mv size
	const float decor_max_dist; // min_dist = MAP_TILE_SIZE

	const std::pair<std::vector<std::pair<std::string, float>>, std::vector<float>> small_decors;
	const std::pair<std::vector<std::string>, std::vector<float>> road_types;

	int num_sur_changes = 0;

	const unsigned cities_count;

	GameObject* add_decor(const std::vector<MapPoint>& map_points);

	BuildingManager* build;

public:
	struct MapSettingsValues {
		MapSettingsValues();

		float decors_per_tile = 1.5f,
			decor_max_dist = 10.0f;

		Ellipse ellipse = { 20, 50, -5, 5 };

		std::pair<std::vector<std::pair<std::string, float>>, std::vector<float>> small_decors = {};
		std::pair<std::vector<std::string>, std::vector<float>> road_types = { {"Asfalt"}, {1} };

		int num_sur_changes = 0;

		uint16_t map_len = 100;
		unsigned checkpoint_offset = 15;
	};

	struct BuildsSettingsValues {
		BuildsSettingsValues();

		unsigned cities_count = 1;
		std::pair<std::array<std::vector<std::string>, 16>, std::vector<float>> buildings;
		std::map<std::string, std::pair<std::vector<std::string>, std::vector<float>>> types;
	};

	// i hate this
	MapManager(size_t seed = 42, const MapSettingsValues& vals = MapSettingsValues(), const BuildsSettingsValues& bvals = BuildsSettingsValues()) :
		rand(seed),
		seed(seed),
		decors_per_tile(vals.decors_per_tile), 
		decor_max_dist(vals.decor_max_dist), 
		small_decors(vals.small_decors),
		road_types(vals.road_types),
		num_sur_changes(vals.num_sur_changes),
		map_len(vals.map_len), 
		cp_offset(vals.checkpoint_offset), 
		ellipse(vals.ellipse),
		cities_count(bvals.cities_count) { 
			build = (new BuildingManager(rand, bvals.buildings.first, bvals.buildings.second))->setCityRoads(bvals.types); 
	};

	MapManager* Init();
	GameObject* GetPoint(long long index) const;
	GameObject* GetCheckPoint(long long index) const;

	unsigned GetLen();
	unsigned GetCheckPoints();
	float GetMapTileSize();
	float GetMapScale();
	std::vector<GameObject*>* GetPoints();
	std::vector<GameObject*>& getCheckPointsObj();
	std::vector<GameObject*>& getDecors();
	BuildingManager* getBuildingManager();

	// -- checkpoints --
	const unsigned cp_offset; // cp stands for checkpoint 💀 

	friend MapDemo;
};

