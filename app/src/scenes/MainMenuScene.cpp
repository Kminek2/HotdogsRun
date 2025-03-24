#include "scenes/MainMenuScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"
#include "objects/CarMovement.h"
#include "objects/WheelsScript.h"

#include "objects/Lights.h"
#include "Sprite.h"

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

    music_first = new AudioSource2d("music/first-intro-accordion", static_cast<float>(Settings::read("volume").value_or(50))/100.0f);
	music_cont = new AudioSource2d("music/continue-intro-accordion", static_cast<float>(Settings::read("volume").value_or(50))/100.0f);

	music_first->PlayTrack(false);
	music_timer = 75.0f;

    cm = new CarMovement(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);

    objs.push_back(new GameObject());
    am = new AnimationManager();
    objs[objs.size()-1]->AddScript(am);
    
    objs.push_back(new GameObject("BaseCube", glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f)));
    objs[objs.size()-1]->AddColorChange(glm::vec3(1.0f), glm::vec3(0.1f, 0.5f, 0.1f));
    
    user_car = new GameObject(car_models[model_choosen], glm::vec3(-6.0f, 0.0f, 0.3f));
    user_car->AddScript(new WheelsScript(*cm));

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
    objs[objs.size()-1]->AddScript(new CarMovement(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false));

    Camera::main->cameraTransform->MoveTo(glm::vec3(-9.96294f, -9.06299f, 2.38608f));
    Camera::main->cameraTransform->RotateTo(glm::vec2(47.0f, -4.0f));
    
    if (first_load) {
        first_animation();
        first_load = false;
    } else {
        ShowMenu();
    }

    LightObject::SetDirLight(glm::vec3(0, 0.5, -0.5), glm::vec3(0.2), glm::vec3(0.7), glm::vec3(0.9));

    Camera::main->ChangeCubeMap(new CubeMap({ "CubeMaps/Skybox/skybox-front.png", "CubeMaps/Skybox/skybox-back.png", "CubeMaps/Skybox/skybox-top.png", "CubeMaps/Skybox/skybox-bottom.png", "CubeMaps/Skybox/skybox-right.png", "CubeMaps/Skybox/skybox-left.png" }));
	return std::shared_ptr<Scene>(scene);
}

void MainMenuScene::Update() {
    music_timer -= Time::deltaTime;
    music_timer = std::max(music_timer, 0.0f);

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
            to_maps_animation();
            //Application::LoadScene("map_demo");
            break;
        case 1:
            to_appearance_animation();
            break;
        case 2:
            
            break;
        case 3:
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
    menu_options[menu_choosen_option].first->SetColor(glm::vec4(0.5254901960784314f,0.7294117647058823f,0.9568627450980392f,1.0f));
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
            HideMaps();
            Application::LoadScene("loading_screen");
            break;
        case 1:    
            HideMaps();
            Application::LoadScene("loading_screen");
            break;
        case 2:
            HideMaps();
            Application::LoadScene("loading_screen"); 
            break;
        }
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
    am->addToQueue({{glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, {glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 1.0f)}, 1.5f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 1.0f)}, {glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 1.0f)}, 0.01f, {0.0f, 0.0f, 0.0f}, true, [](){}, [&](){
        ShowMaps();
    }});
}

void MainMenuScene::from_maps_animation() {
    am->addToQueue({{glm::vec3(16.8101f, 18.9178f, 1.79752f), glm::vec2(88.25f, 1.0f)}, {glm::vec3(15.9145f, 5.02479f, 1.79929f), glm::vec2(62.0f, -7.25f)}, 1.5f, {0.0f, 0.0f, 0.0f}, false, [&](){
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

    if (Input::getKeyClicked(GLFW_KEY_ESCAPE)) {
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
        appearance_options[appearance_options.size()-1].second->SetText(car_models_label[model_choosen]);
        appearance_options[appearance_options.size()-1].first->SetText(car_models_label[model_choosen]);
        user_car->ChangeModel(car_models[model_choosen]);
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->OnDestroy();
        delete user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1];
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1] = new WheelsScript(*cm);
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->gameObject = user_car;
        user_car->GetObjectScripts()[user_car->GetObjectScripts().size()-1]->Init();
    }
}