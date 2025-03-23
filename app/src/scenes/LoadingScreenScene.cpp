#include "scenes/LoadingScreenScene.h"
#include "Application.h"
#include "Scene.h"

const std::vector<std::string> models = {
	"racing_car", "racing_car", "arrow", "racing_car", "zakretPolSkosAsfalt"
};

std::shared_ptr<Scene> LoadingScreenScene::Init() {
	Scene* scene = new Scene(this);
	
	text = new Text("SansSerif", glm::vec3(0.0f), glm::vec2(0.0f), 0.5f);
    text->SetText("Loading...");

	return std::shared_ptr<Scene>(scene);
}

void LoadingScreenScene::Update() {
    Application::LoadScene("map_demo");
}

void LoadingScreenScene::UnLoad() {
	delete text;
}
