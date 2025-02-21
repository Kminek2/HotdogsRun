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

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points(generateMap(map_len, { a,b,min_offset,max_offset }, 42));
	spreadMapPoints(map_points, 5.0f);

	points.reserve(map_points.size());
	
	for (MapPoint point : map_points) {
		GameObject* road = nullptr;

		try { road = road_segements.at({ point.in, point.out }); }
		catch (std::exception& e) { road = road_segements.at({ point.out, point.in }); } // if this crashes out, then something is REALLY wrong

		road->transform->MoveTo({ point.pos.x, point.pos.y, 0 });

		//road = new GameObject("debug_star", { point.pos.x,point.pos.y,0 });

		points.push_back(road);
	}
	
	road_segements.clear();

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc->HandleRotate();
	qc->HandleMove();
}

void MapDemo::UnLoad() {
	points.clear();
	delete qc;
}
