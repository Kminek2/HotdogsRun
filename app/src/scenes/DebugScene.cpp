#include "scenes/DebugScene.h"
#include "Scene.h"
#include "Application.h"
#include "objects/CameraLockScript.h"
#include "objects/LockPosition.h"
#include "objects/LockRotation.h"
#include "objects/CarMovement.h"

const std::vector<std::string> models = {
	"racing_car", "test", "arrow"
};

std::shared_ptr<Scene> DebugScene::Init() {
	Scene* scene = new Scene(this);
	objs.reserve(models.size()+Model::loadedModels.size());

	float position_offset = 0;
	for (const std::string& model : models) {
		objs.push_back(new GameObject(model, {0.0f, position_offset, 0.0f}));
		position_offset += 50.0f;
	}
	position_offset = 0.0f;
	for (const auto model : Model::loadedModels) {
		objs.push_back(new GameObject(model.first, {100.0f, position_offset, 0.0f}));
		position_offset += 50.0f;
	}

	objs[0]->AddScript(new CameraLockScript);
	objs[0]->AddScript(new CarMovement(1.0f, 1.0f, 300.0f, -100.0f, 40.0f, 20.0f, false));

	objs[2]->AddScript(new LockPosition(objs[0]->transform));
	objs[2]->AddScript(new LockRotation(objs[0]->transform));

	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void DebugScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	/*if (Input::getKeyPressed(GLFW_KEY_W))
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
		objs[0]->transform->Rotate(glm::vec3(0.0f, 0.0f, cam_speed * Time::deltaTime));*/

	if (Input::getKeyClicked(GLFW_KEY_R)) {
		Application::LoadScene("load");
		return;
	}
}

void DebugScene::UnLoad() {
	objs.clear();
}
