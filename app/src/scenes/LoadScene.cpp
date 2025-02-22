#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"load", new LoadScene()},
	{"debug", new DebugScene()},
	{"map_demo", new MapDemo()}
};

std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sm(100.0f);

	obj = new GameObject("f1car", { 50, 50, 0 }, glm::vec3(0, 0, 0));
	obj2 = new GameObject("prostaAsfalt", { 0, 0, 0 });

	uiObj = new UiObject("f1car", {0, 0.5, 0}, glm::vec3(0, 0, 0), glm::vec3(0.05));

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void LoadScene::Update() {
	qc->HandleRotate();
	qc->HandleMove();

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
	delete qc;
}