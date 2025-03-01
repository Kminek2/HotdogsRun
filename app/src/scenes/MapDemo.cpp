#include "scenes/MapDemo.h"
#include "Scene.h"
#include "QuickCamera.h"

#include <glm/vec3.hpp>
#include <iostream>

using namespace mapgen;

const size_t seed = 42;

std::shared_ptr<Scene> MapDemo::Init() {
	Scene* scene = new Scene(this);

	qc = new QuickCamera();
	qc->_sr(0.75f);
	qc->_sm(100.0f);

	GameObject* mapObj = new GameObject();
	map = new MapManager(seed);
	mapObj->AddScript(map);

	Camera::main->cameraTransform->MoveTo(map->GetPoint(0)->transform->position);

	return std::shared_ptr<Scene>(scene);
}

void MapDemo::Update() {
	qc->HandleRotate();
	qc->HandleMove();

	if (Input::getKeyPressed(GLFW_KEY_R))
		Application::LoadScene("map_demo");
}

void MapDemo::UnLoad() {
	delete qc;
}
