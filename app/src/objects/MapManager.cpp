#include "MapManager.h"
#include "GameObject.h"
#include "MapManager.h"
#include "MapManager.h"

void MapManager::Init()
{
#pragma region map
	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points = generateMap(map_len, { a,b,min_offset,max_offset }, seed);
	spreadMapPoints(map_points, MAP_TILE_SIZE);

	int n = map_points.size(); // number of map tiles
	points.reserve(n);

	for (MapPoint point : map_points) {
		// if this throws an error, you're looking for a connection that doesn't exist. Refer to `mapgen_road.cpp` or Kamil :)
		ObjectSchema* data = (road_segements.find({ point.in, point.out }) == road_segements.end()
			? road_segements.at({ point.out, point.in })
			: road_segements.at({ point.in, point.out }));

		points.push_back(new GameObject(data, { point.pos.x, point.pos.y, 0 }));
	}

	check_points.reserve(map_points.size() / cp_offset);
	for (const glm::vec2& checkpoint : getCheckPoints(map_points, cp_offset)) {
		check_points.push_back(new GameObject("debug_star", { checkpoint.x, checkpoint.y, 5 }));
	}
#pragma endregion

	unsigned int decors_count = n * decors_per_tile;
	mini_decors.reserve(decors_count);

	_rand rand(seed);

	for (unsigned i = 0; i < decors_count; i++) {
		add_decor(rand, map_points);
	}
}

void MapManager::add_decor(_rand rand, std::vector<MapPoint> map_points) {
	glm::vec2 tile = rand.choice(map_points).pos;

	GameObject* decor = new GameObject(rand.choice(small_decors), {
		tile.x + rand.random(MAP_TILE_SIZE, decor_max_dist),
		tile.y + rand.random(MAP_TILE_SIZE, decor_max_dist), 0 });

	mini_decors.push_back(decor);
}

GameObject* MapManager::GetPoint(unsigned index) {
	return points[index];
}


void MapManager::Update()
{
}

void MapManager::OnDestroy()
{
}