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

using namespace mapgen;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);
	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points(generateMap(map_len, { a,b,min_offset,max_offset }, 42));
	spreadMapPoints(map_points, 255.0f);

	points.reserve(map_points.size());
	
	for (MapPoint point : map_points) {
		GameObject* road = nullptr;

		std::cout << (int)point.in << ' ' << (int)point.out << '\n';

		try { road = road_segements.at({ point.in, point.out }); }
		catch (std::exception& e) { road = road_segements.at({ point.out, point.in }); }

		points.emplace_back(road)->transform->MoveTo({ point.pos.x, 0, point.pos.y });
	}
	
	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc::HandleRotate(0.75f);
	qc::HandleMove(100.0f);
}

void MapDemo::UnLoad() {
	points.clear();
}
