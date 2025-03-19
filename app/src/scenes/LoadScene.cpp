#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"
#include "scenes/MainMenuScene.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"
#include "StringOperations.hpp"

#include "objects/Lights.h"
#include "Sprite.h"
#include "objects/LockPosition.h"
#include "objects/LockRotation.h"

#include <iostream>
#include <filesystem>

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"load", new LoadScene()},
	{"map_demo", new MapDemo()},
	{"main_menu", new MainMenuScene()},
	{"debug", new DebugScene()},
};

std::vector<std::string> LoadScene::preLoadModels = {"hotrod", "Sprite"};
std::vector<std::string> LoadScene::preLoadSprites = {"BaseFont"};


std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);

	Text* loadingText = new Text("SansSerif", { 0, -0.5f, 0 }, glm::vec3(0), 0.5f);
	amountOfLoaded = new Text("SansSerif", { 0, -0.8f, 0 }, glm::vec3(0), 0.5f);

	loadingText->SetText("Loading...");
	count = 0;
	for (auto& p : std::filesystem::directory_iterator("models_obj_test"))
		++count;

	amountOfLoaded->SetText("Loading " + std::to_string(count) + " models...");
	loadingCircle = new GameObject("", {8, 0, 0});
	GameObject* car = new GameObject("hotrod");
	car->AddScript(new LockPosition(loadingCircle->transform, glm::vec3(10, 0, 0)))->AddScript(new LockRotation(loadingCircle->transform, {0, 0, -90}));

	Camera::main->cameraTransform->MoveTo({ 0, 0, 50 });
	Camera::main->cameraTransform->RotateTo({ 0, -90 });
	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void LoadScene::Update() {
	double loadedNum = Model::getLoadedModelCount();

	if (loadedNum > count) {
		Application::LoadScene(scenes[1].first);
		return;
	}

	loadingCircle->transform->Rotate(glm::vec3(0, 0, Time::deltaTime * 100));
	amountOfLoaded->SetText(String::formatDouble(loadedNum / count * 100, 2) + "%");
}

void LoadScene::UnLoad() {}