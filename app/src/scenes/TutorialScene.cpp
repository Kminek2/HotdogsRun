#include "Scene.h"
#include "scenes/TutorialScene.hpp"

std::shared_ptr<Scene> TutorialScene::Init() {
	Scene* scene = new Scene(this);

	Application::SetCursor(true);

	return std::shared_ptr<Scene>(scene);
}

void TutorialScene::Update() {
	if (Input::getKeyClicked(GLFW_KEY_ESCAPE)) {
		Application::LoadScene("main_menu");
		return;
	}
}

void TutorialScene::UnLoad() {}