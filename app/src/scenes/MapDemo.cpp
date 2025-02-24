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
//int n = -1;

//__road road_segements;
//std::vector<MapPoint> map_points;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

	__road road_segements = createRoadMap();

	std::vector<MapPoint> map_points = generateMap(map_len, { a,b,min_offset,max_offset }, 42);
	spreadMapPoints(map_points, 50.0f);

	int n = map_points.size();
	points.reserve(n);
	
	for (MapPoint point : map_points) {
		ObjectSchema* data = nullptr;

		try { data = road_segements.at({ point.in, point.out }); }
		catch (std::exception& e) { data = road_segements.at({ point.out, point.in }); } // if this crashes out, then something is REALLY wrong

		points.push_back(new GameObject(data, { point.pos.x, point.pos.y, 0 }));
	}

	std::cout << n << '\n';

	return std::shared_ptr<Scene>(scene);
}

//int i = 0;
//void MapDemo::add_object() {
//	MapPoint point = map_points[i];
//
//	std::cout << i << ". (" << point.pos.x << ", " << point.pos.y << ") : " << (int)point.in << " | " << (int)point.out << " - ";
//
//	ObjectSchema* data = nullptr;
//
//	try { data = road_segements.at({ point.in, point.out }); }
//	catch (std::exception& e) { data = road_segements.at({ point.out, point.in }); } // if this crashes out, then something is REALLY wrong
//
//	std::cout << data->model << '\n';
//
//	glm::vec3 pos(point.pos.y, point.pos.x, 0);
//	points.push_back(new GameObject(data, pos));
//
//	Camera::main->cameraTransform->MoveTo(pos);
//	++i;
//}

void MapDemo::Update() {
	qc->HandleRotate();
	qc->HandleMove();

	/*if (Input::getKeyClicked(GLFW_KEY_1))
		add_object();*/

	if (Input::getKeyPressed(GLFW_KEY_R))
		Application::LoadScene("map_demo");
}

void MapDemo::UnLoad() {
	delete qc;
}
