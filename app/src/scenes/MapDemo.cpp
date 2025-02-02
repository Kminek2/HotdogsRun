#include "scenes/MapDemo.h"
#include "Scene.h"
#include "mapgen.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <iostream>

const uint16_t map_len = 100;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	std::vector<glm::vec2> map_points(MapGen::generateMap(map_len, 42));
	MapGen::spreadMapPoints(map_points, 10.0f);

	points.reserve(map_points.size());
	
	for (uint16_t i = 0; i < map_points.size(); i++)
		points.push_back(new GameObject("debug_star", glm::vec3({ map_points[i].x, map_points[i].y, i*5})));

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc::HandleRotate(0.75f);
	qc::HandleMove(100.0f);
}

void MapDemo::UnLoad() {
	points.clear();
}
