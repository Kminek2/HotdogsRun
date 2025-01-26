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

	obj2 = new GameObject("test", { 0, 0, 0 });

	obj2->AddScript(new TestObjectScript2());

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void LoadScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		Camera::main->MoveCamera(cam_speed * Time::deltaTime);
	else if (Input::getKeyPressed(GLFW_KEY_S))
		Camera::main->MoveCamera(cam_speed * -Time::deltaTime);

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("debug");
		return;
	}

}

void LoadScene::UnLoad() {
}