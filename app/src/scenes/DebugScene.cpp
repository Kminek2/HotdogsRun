#include "scenes/DebugScene.h"
#include "Scene.h"
#include "Application.h"

std::shared_ptr<Scene> DebugScene::Init() {
	Scene* scene = new Scene(this);
	Input::startKeyCallback();


	return std::shared_ptr<Scene>(scene);
}

void DebugScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		Camera::main->MoveCamera(Time::deltaTime);
	else if (Input::getKeyPressed(GLFW_KEY_S))
		Camera::main->MoveCamera(-Time::deltaTime);

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("load");
		return;
	}
}

void DebugScene::UnLoad() {
	Input::stopKeyCallback();
}