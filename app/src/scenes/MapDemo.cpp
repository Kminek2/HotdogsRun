#include "scenes/MapDemo.h"
#include "AudioSource2d.h"
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

#include <glm/vec3.hpp>
#include <iostream>

#include "objects/StraightKingBot.h"

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

const size_t seed = 45;
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

	_rand rand(seed);
	
	music_type = rand.choice(std::vector<std::string>({"race-accordion", "race-bhrams"}), {0.5f, 0.5f});

	music_first = new AudioSource2d("music/first-"+music_type, static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
	music_cont = new AudioSource2d("music/continue-"+music_type, static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
	sound_race_end = new AudioSource2d("race_end", Settings::read("volume").value_or(50)/100.0f);

	on_end_screen = false;

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

	int model_choosen = Settings::read("model_choosen").value_or(1);
	car = new GameObject(car_models[model_choosen]);

	ColorPicker cp(car);
	cp.update_car();

	car->AddDefaultOBB();
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, 0.1f, false, 0.05f);
		car->AddScript(cmv);
		car->AddScript(new WheelsScript(*cmv, "", 0.9f, 0.9f, 0.0f, 2.2f));
		car->AddScript(new CarInputs(*cmv, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL));
		if (Settings::read("camera_view").value_or(0))
			car->AddScript(new CameraLockScript(Perspective, glm::vec3(-50, 0, 0), -90.0f, 45.0f, false, false, GLFW_KEY_Z, GLFW_KEY_C));
		else
			car->AddScript((new SmoothCamera(glm::vec3(10, 0, 3), 5.0f, Perspective))->SetDynamicFov(true, 45.0f, 50.0f));
	}

	race_manager = (new RaceManager())->SetMapManager(map)->SetEndCondition(3)->SetCarsRelativeOffset(.1f);
	race_manager->SubscribeToRaceEnd([this](RaceManager::CarObject* co) { this->OnRaceEnd(co); });

	race_manager->AddCar(car, true);
	for (int i = 0; i < 1; i++) {
		GameObject* bot = new GameObject("hotrod");

		CarMovement* carmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 150.0f, 20.0f, 0.1f, false, 0.05f);
		bot->AddScript(new WheelsScript(*carmv, "", 0.9f, 0.9f, 0.0f, 2.2f));
		bot->AddScript(carmv)->AddScript(new StraightKingBot(carmv, map));
		//BotMovement* botmv = new BotMovement(carmv);

		//botmv->SetMapManager(map);
		//botmv->GetWaypoints(map);
		//bot->AddScript(botmv)->AddScript(carmv);
		//bot->AddDefaultOBB();

		race_manager->AddCar(bot);
	}

	PowerUp::car_names = race_manager->GetCarNames();

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

	//qc->HandleMove();
	//qc->HandleRotate();
	race_manager->Update();

	if (on_end_screen && UpdateEndScreen()) {
		Application::LoadScene("main_menu");
		return;
	}

	if (Input::getKeyClicked(GLFW_KEY_R))
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