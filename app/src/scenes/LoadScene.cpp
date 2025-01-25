#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "Scene.h"
#include "objects/TestObjectScript.h"
#include "Application.h"
#include "objects/TestObjectScript2.h"

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"load", new LoadScene()},
	{"debug", new DebugScene()}
};

std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);

	Input::startKeyCallback();

	obj = new GameObject("test", { 0, 0, 0 });
	obj2 = new GameObject("test", { 0, 0, -0.5f });

	obj->AddScript(new TestObjectScript());
	obj2->AddScript(new TestObjectScript2());

	return std::shared_ptr<Scene>(scene);
}

void LoadScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		Camera::main->MoveCamera(Time::deltaTime);
	else if (Input::getKeyPressed(GLFW_KEY_S))
		Camera::main->MoveCamera(-Time::deltaTime);

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("debug");
		return;
	}

}

void LoadScene::UnLoad() {
	Input::stopKeyCallback();
}