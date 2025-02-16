#include "scenes/MapDemo.h"
#include "Scene.h"
#include "mapgen.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <iostream>

const uint16_t map_len = 100;

const double a = 20;
const double b = 50;
const double min_offset = -5;
const double max_offset =  5;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	std::vector<mapgen::MapPoint> map_points(MapGen::generateMap(map_len, { a,b,min_offset,max_offset }, 42));
	MapGen::spreadMapPoints(map_points, 25.0f);

	points.reserve(map_points.size());
	
	for (uint16_t i = 0; i < map_points.size(); i++)
		points.push_back(new GameObject("debug_star", glm::vec3({ map_points[i].pos.x, map_points[i].pos.y, i/20.0f})));
	
	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc::HandleRotate(0.75f);
	qc::HandleMove(100.0f);
}

void MapDemo::UnLoad() {
	points.clear();
}
