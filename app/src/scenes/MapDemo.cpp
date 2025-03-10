#include "scenes/MapDemo.h"
#include "Scene.h"
#include "QuickCamera.h"

#include "objects/CameraLockScript.h"
#include "objects/CarInputs.h"
#include "objects/WheelsScript.h"

#include <glm/vec3.hpp>
#include <iostream>

using namespace mapgen;
using tc = RaceManager::TerminationCondition;

const size_t seed = 41;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);
	qc->_mappings(__keybinds({GLFW_KEY_I, GLFW_KEY_K, GLFW_KEY_J, GLFW_KEY_L, GLFW_KEY_U, GLFW_KEY_O, GLFW_KEY_P}));

	_rand rand(seed);

	MapManager::MapSettingsValues svals;
	svals.map_len = 20;
	svals.ellipse.a = 5; svals.ellipse.b = 10;
	svals.ellipse.min_offset = -2.5; svals.ellipse.max_offset = 2.5;
	svals.small_decors = {
		{ "cube", "barrel", "bus", "crate",
			"fucked_up_car", "fucked_up_pickup", "goat", "hydrant",
			"smietnik", "TNT" },
		{ 0.15f, 0.15f, 0.05f, 0.15f,
			0.04f, 0.04f, 0.04f, 0.16f,
			0.12f, 0.1f }
	};

	svals.decors_per_tile = 1.25f;
	svals.decor_max_dist = 5;
	svals.road_types = { {"Asfalt","Zwir","Lod"},{.8,.15,.05} };
	svals.num_sur_changes = 5;

	map = (new MapManager(seed, svals))->Init();

	build = new BuildingManager(seed, { {
		{""}, {"case_1"}, {"case_2"}, {"case_3"},
		{"case_4"}, {"case_5"}, {"case_6"}, {"case_7"},
		{"case_8"}, {"case_9"}, {"case_10"}, {"case_11"},
		{"case_12"}, {"case_13"}, {"case_14"}, {"case_15"}
	} });

	// this makes me want to kms
	std::vector<std::vector<bool>> builds(20);
	for (int j = 0; j < builds.size(); j++) {
		builds[j].resize(builds.size());
		for (int i = 0; i < builds.size(); i++) {
			builds[j][i] = (
				i == 4 || i == 14 || j == 4 || j == 14 || // always on the 4-14 square border
				rand.coin_toss((i == 0 || i == 19 || j == 0 || j == 19) 
					? .1 // 10% on the 0-19 square border
					: ((i < 3 || i > 16 || j < 3 || j > 16) 
						? .4 // 40% outside the 3-16 square
						: .6) // 60% otherwise
				)
			);
		}
	}

	auto vec_builds = build->generateBuildings(builds);

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

	GameObject* car = new GameObject("f1car");
	car->addOBB(OBB());
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, false, 0.05f);
		car->AddScript(cmv);
		car->AddScript(new WheelsScript(*cmv, "3x3_tire_1", 0.9f, 0.9f, 0.0f, 2.2f));
		car->AddScript(new CarInputs(*cmv, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL));
	}

	race_manager = (new RaceManager())->SetMapManager(map)->SetEndCondition(tc::LAPS, 3)->SetCarsRelativeOffset(.1f);
	race_manager->SubscribeToRaceEnd([this](RaceManager::CarObject* co) { this->OnRaceEnd(co); });

	race_manager->AddCar(car);
	for(int i=0; i<3; i++) race_manager->AddCar(new GameObject("f1car"));

	race_manager->StartRace();

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::OnRaceEnd(RaceManager::CarObject* winner) {
	std::cout << "===\n\t" << winner->car->GetModelName() << "\n\t" << winner->checkpoint << "\n\t" << winner->time << '\n';
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
