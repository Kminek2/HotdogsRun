#include "objects/MapManager.h"
#include "GameObject.h"

void MapManager::Init()
{
	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points = generateMap(map_len, ellipse, seed);
	spreadMapPoints(map_points, MAP_TILE_SIZE * MAP_TILE_SCALE);

	int n = map_points.size(); // number of map tiles
	points.reserve(n);

	for (int i = 0; i < n; ++i) {
		MapPoint point = map_points[i];
		ObjectSchema* data;

		if (i < (n-2) && map_points[i+1].in == map_points[i+1].out) {
			data = (road_segements.find({ point.in, map_points[i+2].out }) == road_segements.end()
				? road_segements.at({ map_points[i+2].out, point.in })
				: road_segements.at({ point.in, map_points[i+2].out }));

			i += 2;
		} else {
			// if this throws an error, you're looking for a connection that doesn't exist. Refer to `mapgen_road.cpp` or Kamil :)
			data = (road_segements.find({ point.in, point.out }) == road_segements.end()
				? road_segements.at({ point.out, point.in })
				: road_segements.at({ point.in, point.out }));
		}

		// so that the original doesn't get changed
		data = new ObjectSchema(*data);
		data->scale *= MAP_TILE_SCALE;

		points.push_back(new GameObject(data, { point.pos.x, point.pos.y, 0 }));
	}

	check_points.reserve(points.size() / cp_offset);
	for (const glm::vec2& checkpoint : getCheckPoints(points, cp_offset))
		check_points.push_back(new GameObject("debug_star", { checkpoint.x, checkpoint.y, 5 }));

	unsigned int decors_count = n * decors_per_tile;
	mini_decors.reserve(decors_count);

	_rand rand(seed);

	for (unsigned i = 0; i < decors_count; i++) 
		add_decor(rand, map_points);
}

void MapManager::add_decor(_rand& rand, const std::vector<MapPoint>& map_points) {
	glm::vec2 tile = rand.choice(map_points).pos;

	GameObject* decor = new GameObject(rand.choice(small_decors), {
		tile.x + rand.random(MAP_TILE_SIZE, decor_max_dist) * (rand.coin_toss() ? 1 : -1),
		tile.y + rand.random(MAP_TILE_SIZE, decor_max_dist) * (rand.coin_toss() ? 1 : -1), 0 });

	mini_decors.push_back(decor);
}

/// <summary>
/// Return the pointer to the i-th tile's GameObject
/// </summary>
GameObject* MapManager::GetPoint(unsigned index) {
	return points[index];
}

void MapManager::Update() {}
void MapManager::OnDestroy() {}

MapManager::MapSettingsValues::MapSettingsValues() {}