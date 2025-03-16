#include "scenes/LoadScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"

#include "objects/Lights.h"
#include "Sprite.h"

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
	obj2 = new GameObject({ "prostaAsfalt" }, { 0, 0, 0 });
	obj2->AddColorChange({ 0.93f, 0.93, 0.93 }, { 0.5, 0.5, 0 });
	new GameObject("arrow", { 0, 0, 1 });

	uiObj = new UiObject("f1car", {0, 0.5, 0}, glm::vec3(0, 0, 0), glm::vec3(0.05));

	LightObject::SetDirLight({ glm::vec3(-0.2f, -0.3f, -1.0f), glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(0.04f, 0.04f, 0.04f), glm::vec3(0.05f, 0.05f, 0.05f) });
	LightObject::SetDirLight({ glm::vec3(-0.2f, -0.3f, -1.0f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f) });
	//new PointLight(obj2, { 0, 1, 1 });
	//new PointLight(obj2, { 0, -1, 1 });
	//new SpotLight(obj2, { 0, 0, -1 }, {0, 1, 1});

	GameObject* bilbord = new GameObject("bilboardHodDog", {0, 0, 10});

	bilbord->AddScript(new Lights());

	//DebugPoints::AddLines(debugVerticies, debugIndices);
	//DebugPoints::AddLines({
	//{Camera::main->cameraTransform->position, {0.1f, 0.0f}, {0, 0, 0}},
	//{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(2.05f, -1.55f, 5.6f, 1)), {0.1f, 0.0f}, {0, 0, 0}}}, {0, 1});
	DebugPoints::AddLines({
	{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(1.95f, -1.0f, 5.6f, 1)), {0.6f, 0.0f}, {0, 0, 0}},
	{glm::vec3(bilbord->transform->getModelMatrix() * glm::vec4(2.0f, -1.55f, 5.6f, 1)) + glm::vec3(0, 1, 1), {0.6f, 0.0f}, {0, 0, 0}}}, {0, 1});

	AudioSource2d* testSound = new AudioSource2d("test", 0.5f);
	testSound->PlayTrack(true);

	//AudioSource3d* testSound3d = new AudioSource3d(bilbord, "car-horn-test", 0.2f);
	//testSound3d->PlayTrack(true);
	Application::Invoke(A, 5000);

	Sprite* sp = new Sprite("Roblox");
	sp->rectTransform->MoveTo(glm::vec3(0));
	sp->rectTransform->RotateTo(0);
	sp->rectTransform->ScaleTo(glm::vec2(0.005));

	Text* text = new Text("BaseFont", glm::vec3(0), glm::vec2(0), 0.2f, glm::vec4(0, 1, 0, 0.1f));
	text->SetText("BB!!BB");

	Camera::main->ChangeCubeMap(new CubeMap({ "./CubeMaps/Ranch/posx.jpg", "./CubeMaps/Ranch/negx.jpg", "./CubeMaps/Ranch/posy.jpg", "./CubeMaps/Ranch/negy.jpg", "./CubeMaps/Ranch/posz.jpg", "./CubeMaps/Ranch/negz.jpg" }));

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