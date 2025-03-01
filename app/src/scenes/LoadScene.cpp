#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"load", new LoadScene()},
	{"debug", new DebugScene()},
	{"map_demo", new MapDemo()}
};

const std::vector<Vertex> debugVerticies = {
	{{-0.5f, -0.5f, 2}, {0.1f, 0.0f}, {0, 0, 0}},
	{{0.5f, -0.5f, 2}, {0.1f, 0.0f}, {0, 0, 0}},
	{{0.5f, 0.5f, 2}, {0.1f, 0.0f}, {0, 0, 0}},
	{{-0.5f, 0.5f, 2}, {0.1f, 0.0f}, {0, 0, 0}}
};

const std::vector<uint32_t> debugIndices = {
	0, 1, 1, 2, 2, 3, 3, 0
};


std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sm(100.0f);

	obj = new GameObject("f1car");
	obj2 = new GameObject({"prostaAsfalt"}, {0, 0, 0});
	new GameObject("arrow", { 0, 0, 1 });

	uiObj = new UiObject("f1car", {0, 0.5, 0}, glm::vec3(0, 0, 0), glm::vec3(0.05));

	LightObject::SetDirLight({ glm::vec3(-0.2f, -0.3f, -1.0f), glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(0.04f, 0.04f, 0.04f), glm::vec3(0.05f, 0.05f, 0.05f) });
	//LightObject::SetDirLight({ glm::vec3(-0.2f, -0.3f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f) });
	//new PointLight(obj2, { 0, 1, 1 });
	//new PointLight(obj2, { 0, -1, 1 });
	//new SpotLight(obj2, { 0, 0, -1 }, {0, 1, 1});

	GameObject* bilbord = new GameObject("bilboardHodDog", {0, 0, 10});
	glm::vec3 direction = glm::vec3(0, 1, 1);
	glm::vec3 color = glm::vec3(0.2, 0.2, 0.05);
	new SpotLight(bilbord, { 2.05f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { 1.35f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { 0.65f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { -0.05f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { -0.75f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { -1.45f, -1.55f, 5.6f }, direction, color);
	new SpotLight(bilbord, { -2.15f, -1.55f, 5.6f }, direction, color);

	//DebugPoints::AddLines(debugVerticies, debugIndices);
	//DebugPoints::AddLines({
	//{Camera::main->cameraTransform->position, {0.1f, 0.0f}, {0, 0, 0}},
	//{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(2.05f, -1.55f, 5.6f, 1)), {0.1f, 0.0f}, {0, 0, 0}}}, {0, 1});
	//DebugPoints::AddLines({
	//{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(2.05f, -1.55f, 5.6f, 1)), {0.6f, 0.0f}, {0, 0, 0}},
	//{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(2.05f, -1.55f, 5.6f, 1)) + glm::vec3(0, 1, 1), {0.6f, 0.0f}, {0, 0, 0}}}, {0, 1});

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