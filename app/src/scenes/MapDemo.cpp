﻿#include "scenes/MapDemo.h"
#include "Scene.h"
#include "QuickCamera.h"

#include "objects/CameraLockScript.h"
#include "objects/CarInputs.h"
#include "objects/WheelsScript.h"

#include <glm/vec3.hpp>
#include <iostream>

using namespace mapgen;

const size_t seed = 41;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

	_rand rand(seed);

	MapManager::MapSettingsValues svals;
	svals.map_len = 20;
	svals.ellipse.a = 5; svals.ellipse.b = 10;
	svals.ellipse.min_offset = -2.5; svals.ellipse.max_offset = 2.5;
	svals.small_decors = { {"cube"},{} };
	svals.decors_per_tile = 5;
	svals.decor_max_dist = 5;
	svals.road_types = { {"Asfalt","Zwir","Lod"},{.8,.15,.05} };
	svals.num_sur_changes = 5;

	map = (new MapManager(seed, svals))->Init();

	// -- tmp --
	build = new BuildingManager(seed, { {
		{""}, {"case_1"}, {"case_2"}, {"case_3"},
		{"case_4"}, {"case_5"}, {"case_6"}, {"case_7"},
		{"case_8"}, {"case_9"}, {"case_10"}, {"case_11"},
		{"case_12"}, {"case_13"}, {"case_14"}, {"case_15"}
	} });

	std::vector<std::vector<bool>> builds(20);
	for (int j = 0; j < builds.size(); j++) {
		builds[j].resize(builds.size());
		for (int i = 0; i < builds.size(); i++) {
			builds[j][i] = (
				i == 4 || i == 14 || j == 4 || j == 14 ||
				rand.coin_toss((i == 0 || i == 19 || j == 0 || j == 19) ? .1 : ((i < 3 || i > 16 || j < 3 || j > 16) ? .3 : .5))
			);
		}
	}

	auto vec_builds = build->generateBuildings(builds);
	// -- end_tmp --

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

	//
	/*GameObject* car = new GameObject("f1car");
	car->addOBB(OBB());
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, false, 0.05f);
		car->AddScript(cmv);
		car->AddScript(new WheelsScript(*cmv, "3x3_tire_1", 0.9f, 0.9f, 0.0f, 2.2f));
		car->AddScript(new CarInputs(*cmv, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_PAGE_DOWN));
	}*/
	//car->transform->MoveTo(Camera::main->cameraTransform->position);
	//

	race_manager = (new RaceManager())->SetMapManager(map)->SetCarsRelativeOffset(.1f);
	for(int i=0; i<4; i++) race_manager->AddCar(new GameObject("f1car"));

	GameObject** tmp_debug_objects = new GameObject*[] {
		new GameObject("arrow", map->GetPoint(0)->transform->position),
		new GameObject("debug_star", map->GetPoint(1)->transform->position)
	};

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
	delete build;
	delete race_manager;
	delete map;
}
