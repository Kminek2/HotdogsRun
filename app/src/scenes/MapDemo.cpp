#include "scenes/MapDemo.h"
#include "Scene.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <iostream>

using namespace mapgen;

const size_t seed = 42;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

	MapManager::MapSettingsValues svals;
	svals.small_decors = {
		"cube"
	};

	GameObject* mapObj = new GameObject();
	map = new MapManager(seed, svals);
	mapObj->AddScript(map);

	// -- tmp --
	GameObject* buildObj = new GameObject();
	build = new BuildingManager(seed, { {
		{""}, {"building_1"}, {"building_2"}, {"building_3"},
		{"building_4"}, {"building_5"}, {"building_6"}, {"building_7"},
		{"building_8"}, {"building_9"}, {"building_10"}, {"building_11"},
		{"building_12"}, {"building_13"}, {"debug_star"}, {"debug_star"}
	} });

	_rand rand(seed);
	std::vector<std::vector<bool>> builds(10);
	for (std::vector<bool>& row : builds) {
		row.resize(10);
		for (int i = 0; i < 10; i++)
			row[i] = rand.coin_toss(0.4);
	}

	auto vec_builds = build->generateBuildings(builds);
	// -- end_tmp --

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

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
