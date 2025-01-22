#include "LoadScene.h"
#include "Scene.h"
#include "TestObjectScript.h"

std::vector<std::pair<std::string, SceeneScript*>> LoadScene::scenes = {
	{"test", new LoadScene()}
};

std::shared_ptr<Scene> LoadScene::Init() {
	Scene* scene = new Scene(this);


	obj = new GameObject("test", { 0, 0, 0 });
	obj2 = new GameObject("test", { 0, 0, -0.5f });

	obj->AddScript(new TestObjectScript());

	return std::shared_ptr<Scene>(scene);
}

void LoadScene::Update() {
	Camera::main->RotateCamera(Input::mouseOffX, Input::mouseOffY);

	if (Input::getKeyPressed(GLFW_KEY_W))
		Camera::main->MoveCamera(Time::deltaTime);

	if (Input::getKeyPressed(GLFW_KEY_E))
		obj2->transform->Move(glm::vec3(0, 0, Time::deltaTime));

}

void LoadScene::UnLoad() {

}