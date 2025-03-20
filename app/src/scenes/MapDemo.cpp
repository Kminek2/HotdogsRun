#include "scenes/MapDemo.h"
#include "Scene.h"

#include "objects/CameraLockScript.h"
#include "objects/CarInputs.h"
#include "objects/WheelsScript.h"
#include "objects/ShowOBB.h"
#include "BotMovement.h"
#include "PowerUp.hpp"

#include <glm/vec3.hpp>
#include <iostream>

using namespace mapgen;
using tc = RaceManager::TerminationCondition;

const size_t seed = 45;
const unsigned int cityNum = 3;

const std::array<std::vector<std::string>, 16> defaultBuildings = { {
	{""}, {"case_1","case_1_color"}, {"case_2","case_2_color"}, {"case_3","case_3_color"},
	{"case_4","case_4_color"}, {"case_5"}, {"case_6","case_6_color"}, {"case_7","case_7_color"},
	{"case_8","case_8_color"}, {"case_9","case_9_color"}, {"case_10"}, {"case_11","case_11_color"},
	{"case_12","case_12_color"}, {"case_13","case_13_color"}, {"case_14"}, {"case_15"}
} };

GameObject* car;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	//qc = new QuickCamera();
    //qc->_sr(0.75f);
	//qc->_sm(100.0f);

	on_end_screen = false;

	_rand rand(seed);

	MapManager::MapSettingsValues svals;
	svals.map_len = 20;
	svals.checkpoint_offset = 10;
	svals.ellipse.a = 5; svals.ellipse.b = 10;
	svals.ellipse.min_offset = -2.5; svals.ellipse.max_offset = 2.5;
	svals.small_decors = {
		{		  
			{"fucked_up_car",1}, {"fucked_up_pickup",1}, {"goat",.05}, {"hydrant",.2},
			{"smietnik",.2},{"TNT",0} , {"drzewo",.5}, {"jodla",.5},
			{"kamien1",.8}, {"kamien2",.8},	{"krzak",.8}, {"maleDrzewo",.8},
			{"malyKrzak",.8}, {"pacholki",1}, {"barrel",.2},{"bus",1},
		},
		{	
			-1.0f,-0.75f,-0.25f,0.1f,
			-0.6f,0.05f,-0.8f,-0.8f,
			-0.4f,0.025f,-0.6f,-0.4f,
			-0.25f,-0.3f,0.025f,-0.6f,
		}
	};
	svals.decors_per_tile = 1.25f;
	svals.decor_max_dist = 5;
	svals.road_types = { {"Asfalt","Zwir","Lod"},{.8,.15,.05} };
	svals.num_sur_changes = 5;

	MapManager::BuildsSettingsValues bvals;
	bvals.cities_count = cityNum;
	bvals.buildings = defaultBuildings;
	bvals.types = {
		{"prostaAsfalt", {{"przejscieDlaPieszychAsfalt"},{.1}}},
		{"prostaLod", {{"przejscieDlaPieszychLod"},{.1}}}
	};

	map = (new MapManager(seed, svals, bvals))->Init();

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

	car = new GameObject("f1car");
	car->AddDefaultOBB();
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, 0.1f, false, 0.05f);
		car->AddScript(cmv);
		car->AddScript(new WheelsScript(*cmv, "", 0.9f, 0.9f, 0.0f, 2.2f));
		car->AddScript(new CarInputs(*cmv, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL));
		car->AddScript(new CameraLockScript(Perspective, glm::vec3(-16, 0, 0), -20.0f, 45.0f, true, false, GLFW_KEY_Z, GLFW_KEY_C));
	}

	race_manager = (new RaceManager())->SetMapManager(map)->SetEndCondition(tc::LAPS, 1)->SetCarsRelativeOffset(.1f);
	race_manager->SubscribeToRaceEnd([this](RaceManager::CarObject* co) { this->OnRaceEnd(co); });

	race_manager->AddCar(car);
	for (int i = 0; i < 1; i++) {
		GameObject* bot = new GameObject("hotrod");

		CarMovement* carmv = new CarMovement(1.0f, 1.0f, 300.0f, -100.0f, 80.0f, 20.0f, 0.1f, false, 0.05f);
		BotMovement* botmv = new BotMovement(carmv);

		botmv->SetMapManager(map)->SetCarMovement(carmv);
		botmv->GetWaypoints(map);
		bot->AddScript(botmv)->AddScript(carmv);
		

		race_manager->AddCar(bot);
	}

	PowerUp::car_names = race_manager->GetCarNames();

	GameObject* amobj = new GameObject; // 'am' stands for Animation Manager, apparently
	AnimationManager* am = new AnimationManager;
	amobj->AddScript(am);
	race_manager->SetAnimationManager(am);
	race_manager->StartRace();

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::OnRaceEnd(RaceManager::CarObject* winner) {
	std::cout << "===\n\t" << winner->car->GetModelName() << "\n\t" << winner->checkpoint << "\n\t" << winner->time << '\n';
	Application::Invoke([&](){ShowEndScreen();}, 1000);
}

void MapDemo::Update() {
	//qc->HandleMove();
	//qc->HandleRotate();
	race_manager->Update();

	if (Input::getKeyPressed(GLFW_KEY_R))
		Application::LoadScene("map_demo");

	if (Input::getKeyClicked(GLFW_KEY_B)) {
		std::cout << car->transform->position.x - Camera::main->cameraTransform->position.x << ' ' << car->transform->position.y - Camera::main->cameraTransform->position.y << ' ' << car->transform->position.z - Camera::main->cameraTransform->position.z << '\n' << car->transform->rotation.x - Camera::main->cameraTransform->rotation.x << ' ' << car->transform->rotation.y - Camera::main->cameraTransform->rotation.y << '\n';
	}

	if (on_end_screen && UpdateEndScreen()) {
		Application::LoadScene("main_menu");
		return;
	}
}

void MapDemo::UnLoad() {
	delete build;
	delete race_manager;
	delete map;
}

void MapDemo::ShowEndScreen() {
	on_end_screen = true;
}

bool MapDemo::UpdateEndScreen() {
	if (Input::getKeyClicked(GLFW_KEY_ENTER))
		return true;
	return false;
}