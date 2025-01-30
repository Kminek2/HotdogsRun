#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "Application.h"
#include "QuickCamera.h"

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"load", new LoadScene()},
	{"debug", new DebugScene()},
	{"map_demo", new MapDemo()}
};

std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);

	obj2 = new GameObject("3x3x3", { 0, 0, 0 });

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void LoadScene::Update() {
	qc::HandleRotate();
	qc::HandleMove();

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("debug");
		return;
	}
	if (Input::getKeyClicked(GLFW_KEY_M)) {
		Application::LoadScene("map_demo");
		return;
	}
}

void LoadScene::UnLoad() {
}