#include "scenes/DebugScene.h"
#include "Scene.h"
#include "Application.h"
#include "objects/CameraLockScript.h"
#include "objects/LockToAxis.h"

std::shared_ptr<Scene> DebugScene::Init() {
	Scene* scene = new Scene(this);

	float position_offset = 0;
	for (auto model : Model::loadedModels) {
		objs.push_back(new GameObject(model.first, { 0, position_offset, 0 }));
		position_offset += 50.0f;
	}

	objs[0]->AddScript(new CameraLockScript);
	objs[2]->AddScript([](GameObject* obj) { return new LockToAxis(obj, { 0,0,1 }); }(objs[0]));

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void DebugScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		objs[0]->transform->Translate(glm::vec3(cam_speed * Time::deltaTime, 0.0f, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_S))
		objs[0]->transform->Translate(glm::vec3(cam_speed * -Time::deltaTime, 0.0f, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_A))
		objs[0]->transform->Translate(glm::vec3(0.0f, -cam_speed * Time::deltaTime, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_D))
		objs[0]->transform->Translate(glm::vec3(0.0f, cam_speed * Time::deltaTime, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_SPACE))
		objs[0]->transform->Translate(glm::vec3(0.0f, 0.0f, cam_speed * Time::deltaTime));
	if (Input::getKeyPressed(GLFW_KEY_LEFT_SHIFT))
		objs[0]->transform->Translate(glm::vec3(0.0f, 0.0f, -cam_speed * Time::deltaTime));
	if (Input::getKeyPressed(GLFW_KEY_Z))
		objs[0]->transform->Rotate(glm::vec3(cam_speed * Time::deltaTime, 0.0f, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_X))
		objs[0]->transform->Rotate(glm::vec3(0.0f, cam_speed * Time::deltaTime, 0.0f));
	if (Input::getKeyPressed(GLFW_KEY_C))
		objs[0]->transform->Rotate(glm::vec3(0.0f, 0.0f, cam_speed * Time::deltaTime));

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("load");
		return;
	}
}

void DebugScene::UnLoad() {
	objs.clear();
}