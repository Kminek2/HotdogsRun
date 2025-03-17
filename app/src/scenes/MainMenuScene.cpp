#include "scenes/MainMenuScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"

#include "objects/Lights.h"
#include "Sprite.h"

bool MainMenuScene::first_load = true;

std::shared_ptr<Scene> MainMenuScene::Init() {
	Scene* scene = new Scene(this);

    qc = new QuickCamera();
	qc->_sm(100.0f);

    /*for (int i = 1; i < 14; ++i) {
        objs.push_back(new GameObject("stacja-"+std::to_string(i)));
    }*/
    objs.push_back(new GameObject("stacjaBenzynowaCalosc"));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->RotateTo(glm::vec3(0,0,90.0f));
    objs[objs.size()-1]->transform->Move(glm::vec3(16.8f,1.85f,0));

    ShowMenu();

	return std::shared_ptr<Scene>(scene);
}

void MainMenuScene::Update() {
    qc->HandleRotate();
	qc->HandleMove();
	if (Input::getKeyClicked(GLFW_KEY_Q)) {
		Application::LoadScene("load");
		return;
	}
}

void MainMenuScene::UnLoad() {}

void MainMenuScene::ShowMenu() {
    menu_options.push_back(new Text("SansSerif", {-0.5,-0.5,0.5f}, {-1,-1}, 0.3f));
    //menu_options[menu_options.size()-1]->ChangeSize(0.2f);
    menu_options[menu_options.size()-1]->SetText(";<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
}