#include "objects/MapManager.h"
#include "GameObject.h"

MapManager* MapManager::Init()
{
	_rand rand(seed);

	std::vector<__road> road_segments;
	road_segments.reserve(road_types.first.size());
	for (const std::string& key : road_types.first) road_segments.push_back(createRoadMap(key));

	std::vector<MapPoint> map_points = generateMap(map_len, ellipse, seed);
	spreadMapPoints(map_points, MAP_TILE_SIZE * MAP_TILE_SCALE);

	int n = map_points.size(); // number of map tiles
	points.reserve(n);

	// .9 is a safety margin, because points.size() <= n,
	// but we don't know exactly points.size().
	// it might not work still, so don't stress-test it
	if (num_sur_changes > .9 * n) throw std::invalid_argument("yeah you want to much man");

	std::set<size_t> sur_types_changes = { 0 }; // set of id's of points where the road types change
	int cur_sur_type_id = 0; // current surface type **ID (not type)**
	int _cur_sur_type = -1; // curent surface type
	
	while (sur_types_changes.size() < num_sur_changes) sur_types_changes.insert(rand.random<size_t>(0, static_cast<size_t>(.9 * n)));

	for (int i = 0; i < n; ++i) {
		MapPoint point = map_points[i];
		ObjectSchema* data;

		if (sur_types_changes.find(i) != sur_types_changes.end()) { // change sur-type
			const std::string sur_type = rand.choice(road_types.first, road_types.second);

			cur_sur_type_id = 0;
			while (road_types.first[cur_sur_type_id] != sur_type)
				++cur_sur_type_id;

			_cur_sur_type = ObjectSchema::GetSurfaceType(sur_type);
		}

		if (i < (n-2) && map_points[i+1].in == map_points[i+1].out) {
			data = (road_segments[cur_sur_type_id].find({point.in, map_points[i + 2].out}) == road_segments[cur_sur_type_id].end()
				? road_segments[cur_sur_type_id].at({map_points[i + 2].out, point.in})
				: road_segments[cur_sur_type_id].at({point.in, map_points[i + 2].out}));

			i += 2;
		} else {
			// if this throws an error, you're looking for a connection that doesn't exist. Refer to `mapgen_road.cpp` or Kamil :)
			data = (road_segments[cur_sur_type_id].find({ point.in, point.out }) == road_segments[cur_sur_type_id].end()
				? road_segments[cur_sur_type_id].at({ point.out, point.in })
				: road_segments[cur_sur_type_id].at({ point.in, point.out }));
		}

		// so that the original doesn't get changed
		data = new ObjectSchema(*data);
		data->scale *= MAP_TILE_SCALE;
		data->surface_type = _cur_sur_type;

		points.push_back(new GameObject(data, { point.pos.x, point.pos.y, 0 }));
	}

	check_points.reserve(points.size() / cp_offset);
	for (const glm::vec2& checkpoint : getCheckPoints(points, cp_offset))
		check_points.push_back(new GameObject("debug_star", { checkpoint.x, checkpoint.y, 5 }));

	unsigned int decors_count = n * decors_per_tile;
	mini_decors.reserve(decors_count);

	for (unsigned i = 0; i < decors_count; i++) 
		add_decor(rand, map_points);

	return this;
}

void MapManager::add_decor(_rand& rand, const std::vector<MapPoint>& map_points) {
	glm::vec2 tile = rand.choice(map_points).pos;

	GameObject* decor = new GameObject(rand.choice(small_decors.first, small_decors.second), {
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

/// <summary>
/// Returns the number of road tiles
/// </summary>
int MapManager::GetLen() {
	return points.size();
}

/// <summary>
/// Returns the MAP_TILE_SIZE const
/// </summary>
float MapManager::GetMapTileSize() {
	return MAP_TILE_SIZE;
}

MapManager::MapSettingsValues::MapSettingsValues() {}