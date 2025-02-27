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

	obj = new GameObject("f1car");
	obj2 = new GameObject({"prostaAsfalt"}, {0, 0, 0});
	new GameObject("arrow", { 0, 0, 1 });

	uiObj = new UiObject("f1car", {0, 0.5, 0}, glm::vec3(0, 0, 0), glm::vec3(0.05));

	LightObject::SetDirLight({ glm::vec3(-0.2f, -0.3f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f) });
	new PointLight(obj2, { 0, 0, 1 });

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