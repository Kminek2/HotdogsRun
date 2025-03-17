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
    menu_choosen_option = 0;
    UpdateMenuHighlight();

	return std::shared_ptr<Scene>(scene);
}

void MainMenuScene::Update() {
    UpdateMenu();
    qc->HandleRotate();
	qc->HandleMove();
	if (Input::getKeyClicked(GLFW_KEY_Q)) {
		Application::LoadScene("load");
		return;
	}
}

void MainMenuScene::UnLoad() {}

void MainMenuScene::ShowMenu() {
    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.258f,0.09f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Play");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.25f,0.1f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Play");
    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.258f,-0.21f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Settings");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.25f,-0.2f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Settings");
    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.258f,-0.51f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Exit");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.25f,-0.5f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Exit");
}

void MainMenuScene::UpdateMenu() {
    if (menu_choosen_option < menu_options.size()-1) {
        if (Input::getKeyClicked(GLFW_KEY_DOWN)) {
            ++menu_choosen_option;
            UpdateMenuHighlight();
        }
    }
    if (menu_choosen_option > 0) {
        if (Input::getKeyClicked(GLFW_KEY_UP)) {
            --menu_choosen_option;
            UpdateMenuHighlight();
        }
    }
    if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
        switch (menu_choosen_option)
        {
        case 0:
            Application::LoadScene("map_demo");
            break;
        case 1:
            
            break;
        case 2:
            Application::Quit();
            break;
        }
    }
}

void MainMenuScene::UpdateMenuHighlight() {
    for (int i = 0; i < menu_options.size(); ++i) {
        menu_options[i].first->SetColor(glm::vec4(1.0f));
        menu_options[i].first->ChangeSize(0.4f);
        menu_options[i].second->ChangeSize(0.4f);
    }
    menu_options[menu_choosen_option].first->SetColor(glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f));
    menu_options[menu_choosen_option].first->ChangeSize(0.45f);
    menu_options[menu_choosen_option].second->ChangeSize(0.45f);
}