#include "scenes/DebugScene.h"
#include "Scene.h"
#include "Application.h"
#include "Collisions.h"
#include "objects/CameraLockScript.h"
#include "objects/LockPosition.h"
#include "objects/LockRotation.h"
#include "objects/CarMovement.h"
#include "objects/ShowOBB.h"
#include "objects/CarInputs.h"
#include "objects/WheelsScript.h"
#include "objects/AnimationManager.h"

#include <iostream>

const std::vector<std::string> models = {
	"racing_car", "racing_car", "arrow", "racing_car"
};

std::shared_ptr<Scene> DebugScene::Init() {
	Scene* scene = new Scene(this);
	
	objs.reserve(models.size()+Model::loadedModels.size());

	float position_offset = 0;
	for (const std::string& model : models) {
		objs.push_back(new GameObject(model, {0.0f, position_offset, 0.0f}));
		position_offset += 10.0f;
	}
	position_offset = 0.0f;
	for (const auto model : Model::loadedModels) {
		objs.push_back(new GameObject(model.first, {100.0f, position_offset, 0.0f}));
		position_offset += 10.0f;
	}

	objs[0]->AddScript(new CameraLockScript(Perspective, glm::vec3(-35, 0, 0), -15.0f, 45.0f, true, GLFW_KEY_Z, GLFW_KEY_C));
	
	objs[0]->AddScript(new ShowOBB);
	objs[1]->AddScript(new ShowOBB);
	
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, false);
		objs[0]->AddScript(cmv);
		objs[0]->AddScript(new WheelsScript(*cmv, "3x3_tire_1", 0.9f, 0.9f, 0.0f, 2.2f));
		objs[0]->AddScript(new CarInputs(*cmv));
	}
	
	{
		CarMovement* cmv = new CarMovement(1.0f, 1.0f, 600.0f, -100.0f, 100.0f, 20.0f, false);
		objs[1]->AddScript(cmv);
		objs[1]->AddScript(new WheelsScript(*cmv, "3x3_tire_1", 0.1f, 1.6f, 0.0f, 2.2f));
		objs[1]->AddScript(new CarInputs(*cmv, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_PAGE_DOWN));
	}
	objs[0]->addOBB(OBB(glm::vec3(0,0,0), glm::vec3(1,1,1), {glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)}));
	objs[1]->addOBB(OBB(glm::vec3(0,0,0), glm::vec3(3,3,3), {glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)}));
	
	objs[2]->AddScript(new LockPosition(objs[1]->transform));
	objs[2]->AddScript(new LockRotation(objs[1]->transform));
	objs[2]->AddScript(new LockPosition(objs[3]->transform));
	objs[2]->AddScript(new LockRotation(objs[3]->transform));
	
	AnimationManager* am = new AnimationManager();
	GameObject* am_obj = new GameObject();
	am_obj->AddScript(am);
	am->addToQueue(AnimationManager::data({{-23.0f,-23.0f,9.0f},{45.0f,-15.0f}}, {{-23.0f,-40.0f,9.0f},{45.0f,-15.0f}}, 3.0f, {-20.0f,0.0f,0.0f}));
	am->addToQueue(AnimationManager::data({{-23.0f,-23.0f,9.0f},{45.0f,-15.0f}}, {{-23.0f,-40.0f,9.0f},{45.0f,-15.0f}}, 3.0f, {20.0f,0.0f,0.0f}));
	am->addToQueue(AnimationManager::data({{-23.0f,-23.0f,9.0f},{45.0f,-15.0f}}, {{-23.0f,-40.0f,9.0f},{45.0f,-15.0f}}, 3.0f, {-20.0f,0.0f,0.0f}));
	
	return std::shared_ptr<Scene>(scene);
}

const float cam_speed = 100.0f;
void DebugScene::Update() {
	Camera::main->cameraTransform->Rotate(Input::mouseOff.x, Input::mouseOff.y);

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
		Application::LoadScene("debug");
		return;
	}
	if (Input::getKeyClicked(GLFW_KEY_Q)) {
		Application::LoadScene("load");
		return;
	}
	if (Input::getKeyClicked(GLFW_KEY_ESCAPE)) {
		Application::Quit();
		return;
	}
}

void DebugScene::UnLoad() {
	objs.clear();
}
