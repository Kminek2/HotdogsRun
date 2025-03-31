#include "scenes/MainMenuScene.h"
#include "AudioSource2d.h"
#include "Transform.h"
#include "UiObject.h"
#include "objects/ColorPicker.hpp"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"
#include "scenes/TutorialScene.hpp"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"
#include "objects/CarMovement.h"
#include "objects/WheelsScript.h"

#include "objects/Lights.h"
#include "Sprite.h"
#include "StringOperations.hpp"

const std::string on_off[] = {
    "< OFF >", "< ON >"
};

const std::string camera_view_types[] = {
    "< Third person >", "< Two dimensional >", "< First person >"
};

const std::string car_models[] = {
    "f1car", "hotrod", "pickup", "racing_car"
};

const std::string car_models_label[] = {
    "<  F1 Car  >", "<  Hotrod  >", "<  Pickup  >", "<  Racing Car  >"
};

const std::vector<std::string> colors = {"primary", "secondary", "other"};

bool MainMenuScene::first_load = true;
int MainMenuScene::model_choosen = Settings::read("model_choosen").value_or(0);

std::shared_ptr<Scene> MainMenuScene::Init() {
	Scene* scene = new Scene(this);

    Application::SetCursor(true);

    // qc = new QuickCamera();
    // qc->_sr(0.75f);
	// qc->_sm(100.0f);

    music_timer = 0.0f;

    logo = nullptr;

    volume_delay = 0.0f;

    music_first = new AudioSource2d("music/first-intro-accordion", static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
	music_cont = new AudioSource2d("music/continue-intro-accordion", static_cast<float>(Settings::read("volume").value_or(50))/500.0f);
    sound_select = new AudioSource2d("select", Settings::read("volume").value_or(50)/500.0f);

	music_timer = 0.0f;

    MainMenuScene::model_choosen = Settings::read("model_choosen").value_or(0);

    cm = new CarMovement(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);

    objs.push_back(new GameObject());
    am = new AnimationManager();
    objs[objs.size()-1]->AddScript(am);
    
    objs.push_back(new GameObject("BaseCube", glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f)));
    objs[objs.size()-1]->AddColorChange(glm::vec3(1.0f), glm::vec3(0.1f, 0.5f, 0.1f));
    
    user_car = new GameObject(car_models[model_choosen], glm::vec3(-6.0f, 0.0f, 0.3f));
    user_car->AddScript(new WheelsScript(*cm));

    cp = ColorPicker(user_car);
    cp.update_car();

    objs.push_back(new GameObject("stacjaBenzynowaCalosc"));
    objs.push_back(new GameObject("wjazd"));
    objs[objs.size()-1]->transform->RotateTo(glm::vec3(0.0f,0.0f,180.0f));
    objs[objs.size()-1]->transform->Move(glm::vec3(17.4f, 6.7f, -2.0f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-0.05f, 12.6f, 2.05f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -25.35f, 0.0f));
    objs.push_back(new GameObject("zakretAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -12.7f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 180.0f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-12.7f, 0.0f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 90.0f));
    objs.push_back(new GameObject("wjazd"));
    objs[objs.size()-1]->transform->RotateTo(glm::vec3(0.0f,0.0f,90.0f));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-12.65f, -0.05f, -2.05f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-3]->transform->position + glm::vec3(-25.4f, 0.0f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 90.0f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-12.7f, 0.0f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 90.0f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-12.7f, 0.0f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 90.0f));
    objs.push_back(new GameObject("prostaAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(63.5f, 50.75f, 0.0f));
    objs.push_back(new GameObject("skrzyzowanieTrojkatAsfalt"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, 12.7f, -0.05f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, -90.0f));
    objs.push_back(new GameObject("f1car"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-1.1f, -15.7f, 0.3f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 70.0f));
    objs[objs.size()-1]->AddScript(new WheelsScript(*cm));
    objs.push_back(new GameObject("pickup"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(2.6f, 0.0f, 0.0f));
    objs[objs.size()-1]->transform->Rotate(glm::vec3(0.0f, 0.0f, 70.0f));
    objs[objs.size()-1]->AddScript(new WheelsScript(*cm));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(glm::vec3(6.4f, 14.7f, 0.0f));
    objs.push_back(new GameObject("case_8_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, 6.4f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, 6.4f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, 6.4f, 0.0f));
    objs.push_back(new GameObject("case_1_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, 6.4f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 10.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_2_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-10.0f, -10.0f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_6_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_7_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_3_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-35]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_13_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_9_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_8_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(0.0f, -6.4f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    objs.push_back(new GameObject("case_12_color"));
    objs[objs.size()-1]->transform->Move(objs[objs.size()-2]->transform->position + glm::vec3(-6.4f, 0.0f, 0.0f));
    

    Camera::main->cameraTransform->MoveTo(glm::vec3(-9.96294f, -9.06299f, 2.38608f));
    Camera::main->cameraTransform->RotateTo(glm::vec2(47.0f, -4.0f));
    
    if (first_load) {
        music_first->PlayTrack(false);
	    music_timer = 75.0f;
        first_animation();
        first_load = false;
    } else {
        ShowMenu();
    }

    LightObject::SetDirLight(glm::vec3(0, 0.5, -0.5), glm::vec3(0.2), glm::vec3(0.7), glm::vec3(0.9));

    Camera::main->ChangeCubeMap(new CubeMap({ "CubeMaps/Skybox/skybox-right.png", "CubeMaps/Skybox/skybox-left.png", "CubeMaps/Skybox/skybox-top.png", "CubeMaps/Skybox/skybox-bottom.png", "CubeMaps/Skybox/skybox-front.png", "CubeMaps/Skybox/skybox-back.png" }));
	return std::shared_ptr<Scene>(scene);
}

void MainMenuScene::Update() {
    music_timer -= Time::deltaTime;
    music_timer = std::max(music_timer, 0.0f);

    volume_delay -= Time::deltaTime;
    volume_delay = std::max(volume_delay, 0.0f);

    if (music_timer == 0.0f) {
        music_cont->PlayTrack(false);
        music_timer = 71.0f;
    }

    // qc->HandleMove();
    // qc->HandleRotate();
    if (!menu_options.empty())
        UpdateMenu();
    if (!maps_options.empty())
        UpdateMaps();
    if (!appearance_options.empty())
        UpdateAppearance();
    if (!settings_options.empty())
        UpdateSettings();

    if (Input::getKeyClicked(GLFW_KEY_B)) {
		std::cout << Camera::main->cameraTransform->position.x << ' ' << Camera::main->cameraTransform->position.y << ' ' << Camera::main->cameraTransform->position.z << '\n' << Camera::main->cameraTransform->rotation.x << ' ' << Camera::main->cameraTransform->rotation.y << '\n';
	}
}

void MainMenuScene::UnLoad() {
    delete cm;
}

void MainMenuScene::ShowMenu() {
    logo = new UiObject("logo_pelne", glm::vec3(-0.05f, 0.5f, 0.0f), glm::vec3(90.0f, 180.0f, 180.0f), glm::vec3(0.06f, 0.08f, 0.08f));

    menu_choosen_option = 0;

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,0.19f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Play");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,0.2f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Play");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,-0.11f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Appearance");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,-0.1f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Appearance");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,-0.41f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Settings");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,-0.4f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Settings");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,-0.71f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Exit");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,-0.7f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Exit");
}

void MainMenuScene::HideMenu() {
    for (auto& x : menu_options) {
        delete x.first;
        delete x.second;
    }
    menu_options.clear();
    delete logo;
    logo = nullptr;
}

void MainMenuScene::UpdateMenu() {
    if (Input::getKeyClicked(GLFW_KEY_DOWN) || Input::getKeyClicked(GLFW_KEY_S)) ++menu_choosen_option;
    if (Input::getKeyClicked(GLFW_KEY_UP) || Input::getKeyClicked(GLFW_KEY_W))   --menu_choosen_option;
    
    menu_choosen_option = glm::normalize(menu_choosen_option, static_cast<int>(menu_options.size()));
    UpdateMenuHighlight();

    if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
        switch (menu_choosen_option)
        {
        case 0:
            sound_select->PlayTrack(false);
            to_maps_animation();
            break;
        case 1:
            sound_select->PlayTrack(false);
            to_appearance_animation();
            break;
        case 2:
            sound_select->PlayTrack(false);
            HideMenu();
            ShowSettings();
            break;
        case 3:
            sound_select->PlayTrack(false);
            Application::Quit();
            break;
        }
    }
}

void MainMenuScene::UpdateMenuHighlight() {
    for (int i = 0; i < menu_options.size(); ++i) {
        menu_options[i].first->SetColor(glm::vec4(1.0f));
        menu_options[i].second->ChangeSize(0.4f);
        menu_options[i].first->ChangeSize(0.4f);
    }
    menu_options[menu_choosen_option].second->ChangeSize(0.45f);
    menu_options[menu_choosen_option].first->SetColor(glm::vec4(0.2f,0.5,0.9,1.0f));
    menu_options[menu_choosen_option].first->ChangeSize(0.45f);
}

void MainMenuScene::ShowMaps() {
    menu_choosen_option = 0;

    Sprite* sp = new Sprite("easy");
    sp->rectTransform->SetWidth(0.25f);
    sp->rectTransform->MoveTo(glm::vec3({-0.6f, 0.3f ,0.9f}));
    Text* shadow = (new Text("SansSerif", {-0.79f,-0.31f,0.9f}, {-1,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    shadow->SetText("EASY");
    shadow->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    Text* text = (new Text("SansSerif", {-0.8f,-0.3f,0.8f}, {-1,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    text->SetText("EASY");
    text->SetColor(glm::vec4(glm::vec3(0.0f, 0.7f, 0.0f), 1.0f));
    maps_options.push_back({sp,{text,shadow}});
    
    sp = new Sprite("medium");
    sp->rectTransform->SetWidth(0.25f);
    sp->rectTransform->MoveTo(glm::vec3({0.0f, 0.3f ,0.9f}));
    shadow = (new Text("SansSerif", {0.01f,-0.31f,0.9f}, {0,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    shadow->SetText("MEDIUM");
    shadow->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    text = (new Text("SansSerif", {0.0f,-0.3f,0.9f}, {0,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    text->SetText("MEDIUM");
    text->SetColor(glm::vec4(glm::vec3(0.7f, 0.7f, 0.0f), 1.0f));
    maps_options.push_back({sp,{text,shadow}});
    
    sp = new Sprite("hard");
    sp->rectTransform->SetWidth(0.25f);
    sp->rectTransform->MoveTo(glm::vec3({0.6f, 0.3f ,0.9f}));
    shadow = (new Text("SansSerif", {0.81f,-0.31f,0.9f}, {1,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    shadow->SetText("HARD");
    shadow->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    text = (new Text("SansSerif", {0.8f,-0.3f,0.9f}, {1,0}, 0.4f, glm::vec4(1.0f), -2.0f));
    text->SetText("HARD");
    text->SetColor(glm::vec4(glm::vec3(0.7f, 0.0f, 0.0f), 1.0f));
    maps_options.push_back({sp,{text,shadow}});

    if (!TutorialScene::visited) {
        tutorial_text.second = (new Text("SansSerif", {0.907f,-0.707f,0.9f}, {1,0}, 0.2f, glm::vec4(0,0,0,1.0f), -2.0f))->SetText("Press T for tutorial");
        tutorial_text.first = (new Text("SansSerif", { 0.900f,-0.700f,0.9f }, { 1,0 }, 0.2f, glm::vec4(1.0f), -2.0f))->SetText("Press T for tutorial");
    }

    std::vector<std::string> seed_txt = String::getFile("seed.txt");
    std::string seed = "Seed: " + (seed_txt.empty() ? "[RANDOM]" : String::cropString(seed_txt[0], 10));

    seed_text.second = (new Text("SansSerif", { -.9,-.707,.9 }, { -1,0 }, .2, glm::vec4(0, 0, 0, 1), -2))->SetText(seed);
    seed_text.first = (new Text("SansSerif", { -.9,-.7  ,.9 }, { -1,0 }, .2, glm::vec4(1), -2))->SetText(seed);
}

void MainMenuScene::UpdateMaps() {
    if (Input::getKeyClicked(GLFW_KEY_ESCAPE)) {
        from_maps_animation();
        return;
    }

    if (Input::getKeyClicked(GLFW_KEY_RIGHT)  || Input::getKeyClicked(GLFW_KEY_D)) ++menu_choosen_option;
    if (Input::getKeyClicked(GLFW_KEY_LEFT)  || Input::getKeyClicked(GLFW_KEY_A)) --menu_choosen_option;

    menu_choosen_option = glm::normalize(menu_choosen_option, static_cast<int>(maps_options.size()));
    UpdateMapsHighlight();

    if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
        switch (menu_choosen_option)
        {
        case 0:
            sound_select->PlayTrack(false);
            HideMaps();
            MapDemo::svals.map_len = 20;
            MapDemo::svals.checkpoint_offset = 10;
            MapDemo::svals.ellipse.a = 5; MapDemo::svals.ellipse.b = 10;
            MapDemo::svals.ellipse.min_offset = -2.5; MapDemo::svals.ellipse.max_offset = 2.5;
            MapDemo::svals.small_decors = {
                {		  
                    {"fucked_up_car",0.8}, {"fucked_up_pickup",0.9}, {"goat",.05}, {"hydrant",.2},
                    {"smietnik",.2},{"TNT",0} , {"drzewo",.1}, {"jodla",.1},
                    {"kamien1",.8}, {"kamien2",.4},	{"krzak",.2}, {"maleDrzewo",.2},
                    {"malyKrzak",.4}, {"pacholki",0.9}, {"barrel",.2},{"bus",0.95},
                    {"barierka",0.8},{"bilboardHodDog",0}, {"brzoza",0.01}
                },
                {
                    -1.0f,-0.75f,-0.25f,-0.08f,
                    -0.6f,0.05f,-0.9f,-0.8f,
                    -0.4f,0.025f,-0.7f,-0.3f,
                    -0.4f,-0.3f,0.025f,-0.6f,
                    -0.4f,-0.2f,-0.8f
                }
            };
            MapDemo::svals.decors_per_tile = 1.25f;
            MapDemo::svals.decor_max_dist = 5;
            MapDemo::svals.road_types = { {"Asfalt","Zwir","Lod"},{1.0,.0,.0} };
            MapDemo::svals.num_sur_changes = 5;
            MapDemo::difficulty = 0;
            Application::LoadScene("loading_screen");
            return;
            break;
        case 1:
            sound_select->PlayTrack(false);    
            HideMaps();
            MapDemo::svals.map_len = 20;
            MapDemo::svals.checkpoint_offset = 10;
            MapDemo::svals.ellipse.a = 5; MapDemo::svals.ellipse.b = 10;
            MapDemo::svals.ellipse.min_offset = -2.5; MapDemo::svals.ellipse.max_offset = 2.5;
            MapDemo::svals.small_decors = {
              {
                    {"fucked_up_car",0.8}, {"fucked_up_pickup",0.9}, {"goat",.05}, {"hydrant",.2},
                    {"smietnik",.2},{"TNT",0} , {"drzewo",.1}, {"jodla",.1},
                    {"kamien1",.8}, {"kamien2",.4},	{"krzak",.2}, {"maleDrzewo",.2},
                    {"malyKrzak",.4}, {"pacholki",0.9}, {"barrel",.2},{"bus",0.95},
                    {"barierka",0.8},{"bilboardHodDog",0}, {"brzoza",0.01}
                },
                {
                    -1.0f,-0.75f,-0.25f,-0.08f,
                    -0.6f,0.05f,-0.9f,-0.8f,
                    -0.4f,0.025f,-0.7f,-0.3f,
                    -0.4f,-0.3f,0.025f,-0.6f,
                    -0.4f,-0.2f,-0.8f
                }
            };
            MapDemo::svals.decors_per_tile = 1.25f;
            MapDemo::svals.decor_max_dist = 5;
            MapDemo::svals.road_types = { {"Asfalt","Zwir","Lod"},{.55,.45,.0} };
            MapDemo::svals.num_sur_changes = 5;
            MapDemo::difficulty = 1;
            Application::LoadScene("loading_screen");
            return;
            break;
        case 2:
            sound_select->PlayTrack(false);
            HideMaps();
            MapDemo::svals.map_len = 20;
            MapDemo::svals.checkpoint_offset = 10;
            MapDemo::svals.ellipse.a = 5; MapDemo::svals.ellipse.b = 10;
            MapDemo::svals.ellipse.min_offset = -2.5; MapDemo::svals.ellipse.max_offset = 2.5;
            MapDemo::svals.small_decors = {
                {
                    {"fucked_up_car",0.8}, {"fucked_up_pickup",0.9}, {"goat",.05}, {"hydrant",.2},
                    {"smietnik",.2},{"TNT",0} , {"drzewo",.1}, {"jodla",.1},
                    {"kamien1",.8}, {"kamien2",.4},	{"krzak",.2}, {"maleDrzewo",.2},
                    {"malyKrzak",.4}, {"pacholki",0.9}, {"barrel",.2},{"bus",0.95},
                    {"barierka",0.8},{"bilboardHodDog",0}, {"brzoza",0.01}
                },
                {
                    -1.0f,-0.75f,-0.25f,-0.08f,
                    -0.6f,0.05f,-0.9f,-0.8f,
                    -0.4f,0.025f,-0.7f,-0.3f,
                    -0.4f,-0.3f,0.025f,-0.6f,
                    -0.4f,-0.2f,-0.8f
                }
            };
            MapDemo::svals.decors_per_tile = 1.25f;
            MapDemo::svals.decor_max_dist = 5;
            MapDemo::svals.road_types = { {"Asfalt","Zwir","Lod"},{.7,.15,.15} };
            MapDemo::svals.num_sur_changes = 5;
            MapDemo::difficulty = 2;
            Application::LoadScene("loading_screen");
            return;
            break;
        }
    }

    if (!TutorialScene::visited && Input::getKeyClicked(GLFW_KEY_T)) {
        HideMaps();
        Application::LoadScene("tutorial");
        return;
    }
}

void MainMenuScene::UpdateMapsHighlight() {
    for (int i = 0; i < maps_options.size(); ++i) {
        maps_options[i].second.second->ChangeSize(0.4f);
        maps_options[i].second.first->ChangeSize(0.4f);
        maps_options[i].first->rectTransform->SetWidth(0.25f);
    }
    maps_options[menu_choosen_option].second.second->ChangeSize(0.45f);
    maps_options[menu_choosen_option].second.first->ChangeSize(0.45f);
    maps_options[menu_choosen_option].first->rectTransform->SetWidth(0.3f);
}

void MainMenuScene::HideMaps() {
    for (auto& x : maps_options) {
        delete x.first;
        delete x.second.first;
        delete x.second.second;
    }
    maps_options.clear();
    if (!TutorialScene::visited) {
        delete tutorial_text.first;
        delete tutorial_text.second;
    }
    delete seed_text.first;
    delete seed_text.second;
}

void MainMenuScene::first_animation() {
    am->addToQueue({{glm::vec3(-52.1495f, -18.5977f, 4.1227f), glm::vec2(2.0f, -7.0f)}, {glm::vec3(-19.5279f, -18.6894f, 4.55633f), glm::vec2(29.0f, -6.0f)}, 4.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-19.5279f, -18.6894f, 4.55633f), glm::vec2(29.0f, -6.0f)}, {glm::vec3(-10.50373f, -12.5786f, 3.23154f), glm::vec2(40.0f, -7.0f)}, 2.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-10.50373f, -12.5786f, 3.23154f), glm::vec2(40.0f, -7.0f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 2.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)},{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 0.01f, {0.0f, 0.0f, 0.0f},true,[&](){
        ShowMenu();
        menu_choosen_option = 0;
        UpdateMenuHighlight();
    }});
}

void MainMenuScene::to_maps_animation() {
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, {glm::vec3(-5.16202f, 1.9114f, 2.5676f), glm::vec2(24.5f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}, false, [&](){
        HideMenu();
    }});
    am->addToQueue({{glm::vec3(-5.16202f, 1.9114f, 2.5676f), glm::vec2(24.5f, -2.0f)}, {glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, {glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, 1.5f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, {glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 10.0f)}, 1.5f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 10.0f)}, {glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 10.0f)}, 0.01f, {0.0f, 0.0f, 0.0f}, true, [](){}, [&](){
        ShowMaps();
    }});
}

void MainMenuScene::from_maps_animation() {
    am->addToQueue({{glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 10.0f)}, {glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, 1.5f, {0.0f, 0.0f, 0.0f}, false, [&](){
        HideMaps();
    }});
    am->addToQueue({{glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, {glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, 1.5f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, {glm::vec3(-5.16202f, 1.9114f, 2.5676f), glm::vec2(24.5f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-5.16202f, 1.9114f, 2.5676f), glm::vec2(24.5f, -2.0f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 0.01f, {0.0f, 0.0f, 0.0f}, true, [](){}, [&](){
        ShowMenu();
    }});
}

void MainMenuScene::to_appearance_animation() {
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, {glm::vec3(-7.68801f, -4.80643f, 2.04224f), glm::vec2(60.5f, -10.75f)}, 1.5f, {0.0f, 0.0f, 0.0f}, false, [&](){
        HideMenu();
    }, [&](){
        ShowAppearance();
    }});
}

void MainMenuScene::from_appearance_animation() {
    am->addToQueue({{glm::vec3(-7.68801f, -4.80643f, 2.04224f), glm::vec2(60.5f, -10.75f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 1.5f, {0.0f, 0.0f, 0.0f}, false, [&](){
        HideAppearance();
    }, [&](){
        ShowMenu();
    }});
}

void MainMenuScene::ShowAppearance() {
    appearance_options.push_back({nullptr, nullptr});
    appearance_options[appearance_options.size()-1].second = (new Text("SansSerif", {0.608f,0.49f,0.9f}, {0,0}, 0.3f, glm::vec4(1.0f), -2.0f));
    appearance_options[appearance_options.size()-1].second->SetText(car_models_label[model_choosen]);
    appearance_options[appearance_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    appearance_options[appearance_options.size()-1].first = (new Text("SansSerif", {0.6f,0.5f,0.8f}, {0,0}, 0.3f, glm::vec4(1.0f), -2.0f));
    appearance_options[appearance_options.size()-1].first->SetText(car_models_label[model_choosen]);
    for (int i = 0; i < static_cast<int>(colors.size()); ++i) {
        appearance_options.push_back({nullptr, nullptr});
        appearance_options[appearance_options.size()-1].second = (new Text("SansSerif", {0.608f, 0.1f - (0.4f * static_cast<float>(i)) - 0.01f,0.9f}, {1,-1}, 0.3f, glm::vec4(1.0f), -2.0f));
        appearance_options[appearance_options.size()-1].second->SetText(static_cast<char>(colors[i][0]-32) + colors[i].substr(1, colors[i].length()-1));
        appearance_options[appearance_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
        appearance_options[appearance_options.size()-1].first = (new Text("SansSerif", {0.6f, 0.1f - (0.4f * static_cast<float>(i)),0.8f}, {1,-1}, 0.3f, glm::vec4(1.0f), -2.0f));
        appearance_options[appearance_options.size()-1].first->SetText(static_cast<char>(colors[i][0]-32) + colors[i].substr(1, colors[i].length()-1));
        Sprite* pick = new Sprite("color_wheel");
        pick->rectTransform->Move(glm::vec2(0.8f, 0.1f - (0.4f * static_cast<float>(i))));
        pick->rectTransform->SetHeight(0.2f);
        cp.add_picker(pick, colors[i]);
    }
}

void MainMenuScene::HideAppearance() {
    for (auto& x : appearance_options) {
        delete x.first;
        delete x.second;
    }
    appearance_options.clear();
    cp.close_all_pickers();
}

void MainMenuScene::UpdateAppearance() {
    cp.update();

    if (Input::getKeyClicked(GLFW_KEY_ESCAPE) || Input::getKeyClicked(GLFW_KEY_ENTER)) {
        from_appearance_animation();
        return;
    }

    bool model_changed = false;
    if (Input::getKeyClicked(GLFW_KEY_RIGHT) || Input::getKeyClicked(GLFW_KEY_D)) ++model_choosen, model_changed = true;
    if (Input::getKeyClicked(GLFW_KEY_LEFT) || Input::getKeyClicked(GLFW_KEY_A)) --model_choosen, model_changed = true;

    model_choosen += 4;
    model_choosen %= 4;

    if (model_changed) {
        Settings::save("model_choosen", model_choosen);
        appearance_options[0].second->SetText(car_models_label[model_choosen]);
        appearance_options[0].first->SetText(car_models_label[model_choosen]);
        user_car->ChangeModel(car_models[model_choosen]);
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->OnDestroy();
        delete user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1];
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1] = new WheelsScript(*cm);
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->gameObject = user_car;
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->Init();
        cp.update_car();
    }
}

void MainMenuScene::ShowSettings() {
    menu_choosen_option = 0;

    //BACKGROUND

    settings_bg = new Sprite("settings_bg");
    settings_bg->rectTransform->MoveTo(glm::vec3(0.0f, 0.0f, 1.0f));
    settings_bg->rectTransform->SetWidth(10.0f, false);
    settings_bg->rectTransform->SetHeight(10.0f, false);

    //VOLUME
    settings_options.push_back({{nullptr,nullptr},{nullptr,nullptr}});
    settings_options[settings_options.size()-1].first.second = new Text("SansSerif", glm::vec3(-0.89f, 0.59f, 0.5f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.second->SetText("Volume: ");
    settings_options[settings_options.size()-1].first.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].first.first = new Text("SansSerif", glm::vec3(-0.9f, 0.6f, 0.0f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.first->SetText("Volume: ");
    settings_options[settings_options.size()-1].second.second = new Text("SansSerif", glm::vec3(0.64f, 0.49f, 0.5f), glm::vec2(1.0f, -1.0f), 0.15f);
    settings_options[settings_options.size()-1].second.second->SetText(std::to_string(Settings::read("volume").value_or(50)));
    settings_options[settings_options.size()-1].second.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].second.first = new Text("SansSerif", glm::vec3(0.65f, 0.5f, 0.0f), glm::vec2(1.0f, -1.0f), 0.15f);
    settings_options[settings_options.size()-1].second.first->SetText(std::to_string(Settings::read("volume").value_or(50)));
    volume_bar = new Sprite("range_input_bg");
    volume_bar->rectTransform->SetWidth(0.3f, false);
    volume_bar->rectTransform->SetHeight(0.03f, false);
    volume_bar->rectTransform->MoveTo(glm::vec3(0.35f, 0.6f, 0.6f));
    bar_thumb = new Sprite("range_input_thumb");
    bar_thumb->rectTransform->SetWidth(0.02f);
    bar_thumb->rectTransform->MoveTo(glm::vec3(0.05f+(Settings::read("volume").value_or(50) * 0.006f), 0.6f, 0.5f));

    //CAMERA VIEW
    settings_options.push_back({{nullptr,nullptr},{nullptr,nullptr}});
    settings_options[settings_options.size()-1].first.second = new Text("SansSerif", glm::vec3(-0.89f, 0.29f, 0.5f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.second->SetText("Camera view: ");
    settings_options[settings_options.size()-1].first.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].first.first = new Text("SansSerif", glm::vec3(-0.9f, 0.3f, 0.0f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.first->SetText("Camera view: ");
    settings_options[settings_options.size()-1].second.second = new Text("SansSerif", glm::vec3(0.46f, 0.29f, 0.5f), glm::vec2(0.0f, -1.0f), 0.25f);
    settings_options[settings_options.size()-1].second.second->SetText(camera_view_types[Settings::read("camera_view").value_or(0)]);
    settings_options[settings_options.size()-1].second.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].second.first = new Text("SansSerif", glm::vec3(0.45f, 0.3f, 0.0f), glm::vec2(0.0f, -1.0f), 0.25f);
    settings_options[settings_options.size()-1].second.first->SetText(camera_view_types[Settings::read("camera_view").value_or(0)]);

    //DEBUG
    settings_options.push_back({{nullptr,nullptr},{nullptr,nullptr}});
    settings_options[settings_options.size()-1].first.second = new Text("SansSerif", glm::vec3(-0.89f, -0.01f, 0.5f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.second->SetText("Debug mode: ");
    settings_options[settings_options.size()-1].first.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].first.first = new Text("SansSerif", glm::vec3(-0.9f, 0.0f, 0.0f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.first->SetText("Debug mode: ");
    settings_options[settings_options.size()-1].second.second = new Text("SansSerif", glm::vec3(0.46f, -0.01f, 0.5f), glm::vec2(0.0f, -1.0f), 0.25f);
    settings_options[settings_options.size()-1].second.second->SetText(on_off[Settings::read("debug_mode").value_or(0)]);
    settings_options[settings_options.size()-1].second.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].second.first = new Text("SansSerif", glm::vec3(0.45f, 0.0f, 0.0f), glm::vec2(0.0f, -1.0f), 0.25f);
    settings_options[settings_options.size()-1].second.first->SetText(on_off[Settings::read("debug_mode").value_or(0)]);
    UpdateSettingsHighlight();

    //CLEAR ALL DATA
    settings_options.push_back({{nullptr,nullptr},{nullptr,nullptr}});
    settings_options[settings_options.size()-1].first.second = new Text("SansSerif", glm::vec3(-0.89f, -0.3f, 0.5f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.second->SetText("CLEAR ALL GAME DATA!");
    settings_options[settings_options.size()-1].first.second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    settings_options[settings_options.size()-1].first.first = new Text("SansSerif", glm::vec3(-0.9f, -0.31f, 0.0f), glm::vec2(-1.0f), 0.25f);
    settings_options[settings_options.size()-1].first.first->SetText("CLEAR ALL GAME DATA!");
    UpdateSettingsHighlight();
}

void MainMenuScene::UpdateSettings() {
    if (Input::getKeyClicked(GLFW_KEY_ESCAPE) || Input::getKeyClicked(GLFW_KEY_ENTER)) {
        HideSettings();
        ShowMenu();
        return;
    }

    bool changed = 0;
    if (Input::getKeyClicked(GLFW_KEY_DOWN) || Input::getKeyClicked(GLFW_KEY_S)) ++menu_choosen_option, changed ^= 1;
    if (Input::getKeyClicked(GLFW_KEY_UP) || Input::getKeyClicked(GLFW_KEY_W)) --menu_choosen_option, changed ^= 1;

    menu_choosen_option = glm::normalize(menu_choosen_option, static_cast<int>(settings_options.size()));

    if (changed)
        UpdateSettingsHighlight();

    int suboption_choosen, new_suboption_choosen;
    switch (menu_choosen_option) {
        case 0:
            if (volume_delay != 0.0f)
                break;
            suboption_choosen = Settings::read("volume").value_or(50);
            if (Input::getKeyPressed(GLFW_KEY_RIGHT) || Input::getKeyPressed(GLFW_KEY_D)) Settings::save("volume", suboption_choosen+1), volume_delay = 0.1f;
            if (Input::getKeyPressed(GLFW_KEY_LEFT) || Input::getKeyPressed(GLFW_KEY_A)) Settings::save("volume", suboption_choosen-1), volume_delay = 0.1f;
            new_suboption_choosen = Settings::read("volume").value_or(50);
            if (new_suboption_choosen < 0) Settings::save("volume", 0), new_suboption_choosen = 0;
            if (new_suboption_choosen > 100) Settings::save("volume", 100), new_suboption_choosen = 100;
            if (suboption_choosen != new_suboption_choosen) {
                settings_options[0].second.second->SetText(std::to_string(new_suboption_choosen));
                settings_options[0].second.first->SetText(std::to_string(new_suboption_choosen));
                bar_thumb->rectTransform->MoveTo(glm::vec3(0.05f+(new_suboption_choosen * 0.006f), 0.6f, 0.5f));
                music_first->setVolume(new_suboption_choosen / 500.0f);
                music_cont->setVolume(new_suboption_choosen / 500.0f);
                sound_select->setVolume(new_suboption_choosen / 100.0f);
            }
            break;
        case 1:
            suboption_choosen = Settings::read("camera_view").value_or(0);
            new_suboption_choosen = suboption_choosen;
            if (Input::getKeyClicked(GLFW_KEY_RIGHT) || Input::getKeyClicked(GLFW_KEY_D)) ++new_suboption_choosen;
            if (Input::getKeyClicked(GLFW_KEY_LEFT) || Input::getKeyClicked(GLFW_KEY_A)) --new_suboption_choosen;
            new_suboption_choosen = glm::normalize(new_suboption_choosen, 3);
            if (suboption_choosen != new_suboption_choosen) {
                Settings::save("camera_view", new_suboption_choosen);
                settings_options[1].second.second->SetText(camera_view_types[new_suboption_choosen]);
                settings_options[1].second.first->SetText(camera_view_types[new_suboption_choosen]);
            }
            break;
        case 2:
            suboption_choosen = Settings::read("debug_mode").value_or(0);
            new_suboption_choosen = suboption_choosen;
            if (Input::getKeyClicked(GLFW_KEY_RIGHT) || Input::getKeyClicked(GLFW_KEY_D)) ++new_suboption_choosen;
            if (Input::getKeyClicked(GLFW_KEY_LEFT) || Input::getKeyClicked(GLFW_KEY_A)) --new_suboption_choosen;
            new_suboption_choosen = glm::normalize(new_suboption_choosen, 2);
            if (suboption_choosen != new_suboption_choosen) {
                Settings::save("debug_mode", new_suboption_choosen);
                settings_options[2].second.second->SetText(on_off[new_suboption_choosen]);
                settings_options[2].second.first->SetText(on_off[new_suboption_choosen]);
            }
            break;
        case 3:
            if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
                sound_select->PlayTrack(false);
                Settings::clear();
                cp.update_car();
                HideSettings();
                ShowMenu();
                return;
            }
            break;
    }
}

void MainMenuScene::HideSettings() {
    for (auto& x : settings_options) {
        if (x.first.first != nullptr)
            delete x.first.first;
        if (x.first.second != nullptr)
            delete x.first.second;
        if (x.second.first != nullptr)
            delete x.second.first;
        if (x.second.second != nullptr)
            delete x.second.second;
    }
    settings_options.clear();
    delete volume_bar;
    delete bar_thumb;
    delete settings_bg;
}

void MainMenuScene::UpdateSettingsHighlight() {
    for (int i = 0; i < settings_options.size(); ++i) {
        settings_options[i].first.first->SetColor(glm::vec4(1.0f));
        settings_options[i].first.second->ChangeSize(0.25f);
        settings_options[i].first.first->ChangeSize(0.25f);
    }
    settings_options[menu_choosen_option].first.second->ChangeSize(0.27f);
    settings_options[menu_choosen_option].first.first->SetColor(glm::vec4(0.5254901960784314f,0.7294117647058823f,0.9568627450980392f,1.0f));
    settings_options[menu_choosen_option].first.first->ChangeSize(0.27f);
}