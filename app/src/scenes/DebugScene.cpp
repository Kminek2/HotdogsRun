#include "scenes/DebugScene.h"
#include "Scene.h"
#include "Application.h"
#include "objects/CameraLockScript.h"

std::shared_ptr<Scene> DebugScene::Init() {
	Scene* scene = new Scene(this);

	Camera::main->view = Isometric;
	float position_offset = 0;
	for (auto model : Model::loadedModels) {
		objs.push_back(new GameObject(model.first, { 0, position_offset, 0 }));
		position_offset += 50.0f;
	}

	objs[0]->AddScript(new CameraLockScript(Perspective, glm::vec3(-100.0f, 0.0f, 0.0f), 0.0f, 0.0f));

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void DebugScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		Camera::main->MoveCamera(cam_speed*Time::deltaTime);
	else if (Input::getKeyPressed(GLFW_KEY_S))
		Camera::main->MoveCamera(cam_speed*-Time::deltaTime);

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("load");
		return;
	}
}

void DebugScene::UnLoad() {
	objs.clear();
}