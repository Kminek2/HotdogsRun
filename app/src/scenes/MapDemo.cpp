#include "scenes/MapDemo.h"
#include "AudioSource2d.h"
#include "CameraStructures.h"
#include "GameObject.h"
#include "Scene.h"

#include "objects/ColorPicker.hpp"
#include "objects/SmoothCamera.hpp"
#include "objects/CameraLockScript.h"
#include "objects/CarInputs.h"
#include "objects/WheelsScript.h"
#include "objects/ShowOBB.h"
#include "BotMovement.h"
#include "PowerUp.hpp"
#include "objects/PUManager.hpp"
#include "_rand.hpp"

#include <exception>
#include <glm/vec3.hpp>
#include <iostream>
#include <random>

#include "objects/StraightKingBot.h"
#include "objects/MedBot.h"

using namespace mapgen;

const std::string car_models[] = {
    "f1car", "hotrod", "pickup", "racing_car"
};

const std::vector<std::vector<glm::vec3>> default_colors = {
    {
		glm::vec3(0.9686274509803922f, 0.37254901960784315f, 0.17647058823529413f),
		glm::vec3(0.8666666666666667f, 0.8666666666666667f, 0.8666666666666667f),
		glm::vec3(0.43137254901960786f, 0.43137254901960786f, 0.43137254901960786f)
	},
	{
		glm::vec3(0.26666666666666666f, 0.26666666666666666f, 0.26666666666666666f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	},
	{
		glm::vec3(0.9333333333333333f, 0.0f, 0.0f),
		glm::vec3(0.3333333333333333f, 0.3333333333333333f, 0.3333333333333333f),
		glm::vec3(0.9333333333333333f, 0.9333333333333333f, 0.9333333333333333f)
	},
	{
		glm::vec3(0.0f, 0.4f, 0.8f),
		glm::vec3(0.9333333333333333f, 0.9333333333333333f, 0.9333333333333333f),
		glm::vec3(0.06666666666666667f, 0.06666666666666667f, 0.06666666666666667f)
	}
};

const std::vector<std::string> colors = {"primary", "secondary", "other"};

MapManager::MapSettingsValues MapDemo::svals;
int MapDemo::difficulty;

//size_t seed = 123306034;
const unsigned int cityNum = 3;

const std::pair<std::array<std::vector<std::string>, 16>, std::vector<float>> defaultBuildings = { { {
	{""}, {"case_1","case_1_color"}, {"case_2","case_2_color"}, {"case_3","case_3_color"},
	{"case_4","case_4_color"}, {"case_5","case_5_color"}, {"case_6","case_6_color"}, {"case_7","case_7_color"},
	{"case_8","case_8_color"}, {"case_9","case_9_color"}, {"case_10","case_10_color"}, {"case_11","case_11_color"},
	{"case_12","case_12_color"}, {"case_13","case_13_color"}, {"case_14","case_14_color"}, {"case_15","case_15_color"}
} }, {.1f, -1.0f} };

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	//qc = new QuickCamera();
	//qc->_sr(0.75f);
	//qc->_sm(100.0f);

	music_timer = 0.0f;
	first_music = true;

	//_rand rand(seed);
	_rand rand;

	std::cout << "USING SEED: " << rand.getSeed() << '\n';
	
	music_type = rand.choice(std::vector<std::string>({"race-accordion", "race-bhrams"}), {0.5f, 0.5f});

	music_first = new AudioSource2d("music/first-"+music_type, static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
	music_cont = new AudioSource2d("music/continue-"+music_type, static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
	sound_race_end = new AudioSource2d("race_end", Settings::read("volume").value_or(50)/100.0f);

	on_end_screen = false;

	MapManager::BuildsSettingsValues bvals;
	bvals.cities_count = cityNum;
	bvals.buildings = defaultBuildings;
	bvals.types = {
		{"prostaAsfalt", {{"przejscieDlaPieszychAsfalt"},{.1}}},
		{"prostaLod", {{"przejscieDlaPieszychLod"},{.1}}}
	};

	bool done = false;
	for (int i = 0; !done; ++i) {
		try {
			map = (new MapManager(rand.getSeed() + i, svals, bvals))->Init();
			done = true;
		} catch (std::exception e) {
			GameObject::DeleteAll();
		}
	}

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

	int model_choosen = Settings::read("model_choosen").value_or(1);
	car = new GameObject(car_models[model_choosen]);

	ColorPicker cp(car);
	cp.update_car();

	camera_view = Settings::read("camera_view").value_or(0);
	car->AddDefaultOBB();
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, 0.1f, false, 0.05f);
		car->AddScript(cmv);
		car->AddScript(new WheelsScript(*cmv, "", 0.9f, 0.9f, 0.0f, 2.2f));
		car->AddScript(new CarInputs(*cmv, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL));
		view_scripts[0] = new CameraLockScript(Perspective, glm::vec3(-50, 0, 0), -90.0f, 45.0f, false, false, GLFW_KEY_Z, GLFW_KEY_C);
		view_scripts[1] = new CameraLockScript(Perspective, glm::vec3(1.7f, 0.0f, 1.5f), 0.0f, 90.0f, true, false, GLFW_KEY_Z, GLFW_KEY_C);
		car->AddScript(view_scripts[0]);
		car->AddScript((new SmoothCamera(glm::vec3(10, 0, 3), 5.0f, Perspective))->SetDynamicFov(true, 45.0f, 50.0f));
		car->AddScript(view_scripts[1]);
	}

	if (camera_view != 0)
		SmoothCamera::disabled2 = true;
	if (camera_view != 1)
		view_scripts[0]->disabled2 = true;
	if (camera_view != 2)
		view_scripts[1]->disabled2 = true;

	race_manager = (new RaceManager())->SetMapManager(map)->SetEndCondition(3)->SetCarsRelativeOffset(.1f);
	race_manager->SubscribeToRaceEnd([this](RaceManager::CarObject* co) { this->OnRaceEnd(co); });

	race_manager->AddCar(car, true);
	for (int i = 0; i < 1; i++) {
		GameObject* bot = new GameObject("hotrod");

		CarMovement* carmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 150.0f, 20.0f, 0.1f, false, 0.05f * (1 + (difficulty + 1) / 10.0f));
		bot->AddScript(new WheelsScript(*carmv, "", 0.9f, 0.9f, 0.0f, 2.2f));

		RaceManager::CarObject* carObj = race_manager->AddCar(bot);

		bot->AddScript(carmv)->AddScript(new StraightKingBot(carmv, map, carObj));
		/*if (difficulty == 0)
			bot->AddScript(carmv)->AddScript(new StraightKingBot(carmv, map, carObj));
		else if(difficulty == 1)
			bot->AddScript(carmv)->AddScript(new StraightKingBot(carmv, map, carObj, 0.5f, 0.3f));
		else if(difficulty == 2)
			bot->AddScript(carmv)->AddScript(new StraightKingBot(carmv, map, carObj, 0.5f, 0.4f));*/

		bot = new GameObject("f1car");

		carmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 150.0f, 20.0f, 0.1f, false, 0.05f * (1 + (difficulty + 1) / 3.0f));
		bot->AddScript(new WheelsScript(*carmv, "", 0.9f, 0.9f, 0.0f, 2.2f));

		carObj = race_manager->AddCar(bot);
		bot->AddScript(carmv)->AddScript(new MedBot(carmv, map, carObj));
		/*if (difficulty == 0)
			bot->AddScript(carmv)->AddScript(new MedBot(carmv, map, carObj));
		else if(difficulty == 1)
			bot->AddScript(carmv)->AddScript(new MedBot(carmv, map, carObj, 0.8f));
		else if(difficulty == 2)
			bot->AddScript(carmv)->AddScript(new MedBot(carmv, map, carObj, 0.9f, 0.6f, 0.3f));*/
	}

	PowerUp::car_objects = race_manager->getCarObjects();

	PUManager* pum = (new PUManager(rand))->setMapManager(map);
	pum->addPowerUp((new GameObject("cube", glm::vec3(1000.0f)))->AddScript(new PUNitro()), -1.0f);
	pum->addPowerUp((new GameObject("crate", glm::vec3(1000.0f)))->AddScript(new PUMaxSpeed(.1f, 10 * 1000)), -1.0f);
	pum->generatePowerUps(15);

	GameObject* amobj = new GameObject; // 'am' stands for Animation Manager, apparently
	AnimationManager* am = new AnimationManager;
	amobj->AddScript(am);
	race_manager->SetAnimationManager(am);
	race_manager->StartRace();
	
	music_first->PlayTrack(false);
	if (music_type == "race-accordion")
		music_timer = 243.0f;
	else
		music_timer = 138.0f;

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::OnRaceEnd(RaceManager::CarObject* winner) {
	sound_race_end->PlayTrack(false);
	std::cout << "===\n\t" << winner->car->GetModelName() << "\n\t" << winner->checkpoint << "\n\t" << winner->time << '\n';
	Application::Invoke([&](){ShowEndScreen();}, 10);
}

void MapDemo::Update() {
	music_timer -= Time::deltaTime;
	music_timer = std::max(music_timer, 0.0f);

	if (music_timer == 0.0f) {
		music_cont->PlayTrack(false);
		if (music_type == "race-accordion")
			music_timer = 241.0f;
		else
			music_timer = 136.0f;
	}

	if (Input::getKeyClicked(GLFW_KEY_C)) {
		camera_view = glm::normalize(camera_view + 1, 3);

		SmoothCamera::disabled2 = false;
		view_scripts[0]->disabled2 = false;
		view_scripts[1]->disabled2 = false;

		if (camera_view != 0)
			SmoothCamera::disabled2 = true;
		if (camera_view != 1)
			view_scripts[0]->disabled2 = true;
		if (camera_view != 2)
			view_scripts[1]->disabled2 = true;
	}

	//qc->HandleMove();
	//qc->HandleRotate();
	race_manager->Update();

	if (on_end_screen && UpdateEndScreen()) {
		Application::LoadScene("main_menu");
		return;
	}

	if (Settings::read("debug_mode").value_or(0) && Input::getKeyClicked(GLFW_KEY_R))
		race_manager->EndRace();
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