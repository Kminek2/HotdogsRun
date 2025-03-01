#include "scenes/MapDemo.h"
#include "Scene.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <iostream>

// -- general --
const size_t seed = 42;

// -- road --
const uint16_t map_len = 100;

const double a = 20;
const double b = 50;
const double min_offset = -5;
const double max_offset =  5;

// -- checkpoints --
const unsigned cp_offset = 15; // cp stands for checkpoint 💀

// -- decors -- 
const double decors_per_tile = 1.5f;
const float decor_max_dist = 20.0f; // min_dist = MAP_TILE_SIZE

const std::vector<std::string> small_decors = {
	"cube"
};

using namespace mapgen;

void MapDemo::add_decor(_rand rand, std::vector<mapgen::MapPoint> map_points) {
	glm::vec2 tile = rand.choice(map_points).pos;

	GameObject* decor = new GameObject(rand.choice(small_decors), {
		tile.x + rand.random(MAP_TILE_SIZE, decor_max_dist), 
		tile.y + rand.random(MAP_TILE_SIZE, decor_max_dist), 0});

	mini_decors.push_back(decor);
}

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

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

	Camera::main->cameraTransform->MoveTo(points[0]->transform->position);

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc->HandleRotate();
	qc->HandleMove();

	if (Input::getKeyPressed(GLFW_KEY_R))
		Application::LoadScene("map_demo");
}

void MapDemo::UnLoad() {
	delete qc;
}
