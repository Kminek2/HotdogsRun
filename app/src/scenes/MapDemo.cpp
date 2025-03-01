#include "scenes/MapDemo.h"
#include "Scene.h"
#include "mapgen.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <random>
#include <iostream>

const uint16_t map_len = 100;

const double a = 20;
const double b = 50;
const double min_offset = -5;
const double max_offset =  5;

const unsigned cp_offset = 15; // cp stands for checkpoint 💀

const double decors_per_tile = 1.5f;

const std::vector<std::string> small_decors = {
	"cube"
};

using namespace mapgen;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

#pragma region init qc
	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);
#pragma endregion

#pragma region map
	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points = generateMap(map_len, { a,b,min_offset,max_offset }, 42);
	spreadMapPoints(map_points, 50.0f);

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
	std::random_device rd;

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
