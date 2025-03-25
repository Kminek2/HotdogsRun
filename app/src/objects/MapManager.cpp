#include "objects/MapManager.h"
#include "GameObject.h"
#include "objects/ShowOBB.h"
#include <exception>

MapManager* MapManager::Init()
{
	std::vector<__road> road_segments;
	road_segments.reserve(road_types.first.size());
	for (const std::string& key : road_types.first) road_segments.push_back(createRoadMap(key));

	std::vector<MapPoint> map_points;
	bool not_done = true;
	for (int i = 0; not_done; ++i) {
		try {
			map_points = generateMap(map_len, ellipse, seed+i);
			spreadMapPoints(map_points, MAP_TILE_SIZE * MAP_TILE_SCALE);
			not_done = false;
		} catch (std::exception e) {}
	}

	int n = map_points.size(); // number of map tiles
	points.reserve(n);

	// .9 is a safety margin, because points.size() <= n,
	// but we don't know exactly points.size().
	// it might not work still, so don't stress-test it
	if (num_sur_changes > .9 * n) throw std::invalid_argument("yeah you want too much man");

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
			data = (road_segments[cur_sur_type_id].find({ point.in, point.out }) == road_segments[cur_sur_type_id].end()
				? road_segments[cur_sur_type_id].at({ point.out, point.in })
				: road_segments[cur_sur_type_id].at({ point.in, point.out }));
		}

		// so that the original doesn't get changed
		data = new ObjectSchema(*data);
		data->scale *= MAP_TILE_SCALE;
		data->surface_type = _cur_sur_type;

		points.push_back(new GameObject(data, { point.pos.x, point.pos.y, -0.3f }));
		points[points.size()-1]->AddDefaultOBB({0.0f, 0.0f, 0.0f}, true);
	}

	std::vector<glm::vec2> checkpoint_positions = getCheckPoints(points, cp_offset);
	check_points.reserve(checkpoint_positions.size());
	for (int i=0; i< checkpoint_positions.size(); i++) {
		check_points.push_back(new GameObject(
			"checkpoint", 
			{ checkpoint_positions[i].x, checkpoint_positions[i].y, 0}, 
			GetPoint(i * cp_offset)->transform->rotation, 
			glm::vec3(MAP_TILE_SCALE)));
		check_points[check_points.size()-1]->transform->Translate(glm::vec3(0.0f, -(MAP_TILE_SIZE*MAP_TILE_SCALE*0.5f), 0.0f));
	}

	unsigned int decors_count = n * decors_per_tile;

	for (unsigned i = 0; i < decors_count; i++) 
		decors.push_back(add_decor(map_points));

	build->setMap(&points);
	build->generateCities(cities_count)->replaceCityRoads();

	(new GameObject("BaseCube", glm::vec3(0, 0, -0.23f), glm::vec3(0), glm::vec3(1000, 1000, 0)))->AddColorChange(glm::vec3(1), glm::vec3(0.1f, 0.5f, 0.1f)); // just TEMPORARY hradcoded ground

	return this;
}

const std::array<glm::vec2, 8> directions = { {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}} };
GameObject* MapManager::add_decor(const std::vector<MapPoint>& map_points) {
	glm::vec2 tile = rand.choice(map_points).pos;

	std::pair<std::string, float> obj_data = rand.choice(small_decors.first, small_decors.second);
	GameObject* decor = new GameObject(obj_data.first, {
		tile.x + rand.random(MAP_TILE_SIZE, decor_max_dist) * (rand.coin_toss() ? 1 : -1),
		tile.y + rand.random(MAP_TILE_SIZE, decor_max_dist) * (rand.coin_toss() ? 1 : -1), -0.2f }, { 0,0,rand.random(0.0f,360.0f) }, glm::vec3(MAP_TILE_SCALE));
	if (decor->GetModelName() == "fucked_up_car" || decor->GetModelName() == "fucked_up_pickup" || decor->GetModelName() == "bus" || decor->GetModelName() == "goat")
		decor->transform->ScaleTo(glm::vec3(MAP_TILE_SCALE/2.0f));
	else
		decor->transform->ScaleTo(glm::vec3(MAP_TILE_SCALE/1.2f));
		

	decor->AddDefaultOBB();

	if (rand.coin_toss(obj_data.second)) return decor; // allow for on-road placement if TRUE

	int bef, cur, nxt;
	bef = cur = nxt = -1;

	for (size_t i = 0; i < points.size(); i++) {
		if (!Collisions::checkCollision(*points[i], *decor)) continue;

		bef = glm::normalize(i - 1, points.size());
		cur = i;
		nxt = glm::normalize(i + 1, points.size());

		break;
	}

	if (cur == -1) return decor; // no collision - all good!

	glm::vec3 pos = decor->transform->position;
	for (glm::vec2 dir : directions) {
		decor->transform->Move(glm::vec3(dir.x, dir.y, 0) * MAP_TILE_SIZE * MAP_TILE_SCALE);
		if (!Collisions::checkCollision(*points[bef], *decor) &&
			!Collisions::checkCollision(*points[cur], *decor) &&
			!Collisions::checkCollision(*points[nxt], *decor))
			break;

		decor->transform->MoveTo(pos);
	}
	return decor;
}

/// <summary>
/// Return the pointer to the i-th tile's GameObject
/// </summary>
GameObject* MapManager::GetPoint(unsigned long long index) {
	return points[glm::normalize(index, static_cast<unsigned long long>(points.size()))];
}

/// <summary>
/// Return the pointer to the i-th checkpoint's GameObject
/// </summary>
GameObject* MapManager::GetCheckPoint(unsigned long long index) {
	return check_points[glm::normalize(index, static_cast<unsigned long long>(check_points.size()))];
}

/// <summary>
/// Returns the number of road tiles
/// </summary>
unsigned MapManager::GetLen() {
	return points.size();
}

/// <summary>
/// Returns the number of checkpoints
/// </summary>
unsigned MapManager::GetCheckPoints() {
	return check_points.size();
}

/// <summary>
/// Returns the MAP_TILE_SIZE const
/// </summary>
float MapManager::GetMapTileSize() {
	return MAP_TILE_SIZE;
}

/// <summary>
/// Returns the MAP_TILE_SCALE const
/// </summary>
float MapManager::GetMapScale() {
	return MAP_TILE_SCALE;
}

/// <summary>
/// Returns the reference (pointer) to the whole vector of map tiles (unadviced to use).
/// </summary>
std::vector<GameObject*>* MapManager::GetPoints()
{
	return &points;
}

MapManager::MapSettingsValues::MapSettingsValues() {}
MapManager::BuildsSettingsValues::BuildsSettingsValues() {}

BuildingManager* MapManager::getBuildingManager() {
	return build;
}

std::vector<GameObject*>& MapManager::getDecors() {
	return decors;
}