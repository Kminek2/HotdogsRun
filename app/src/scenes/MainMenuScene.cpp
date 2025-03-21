#include "scenes/MainMenuScene.h"
#include "scenes/DebugScene.h"
#include "scenes/MapDemo.h"

#include "Scene.h"
#include "QuickCamera.h"
#include "DebugPoints.h"
#include "objects/CarMovement.h"

#include "objects/Lights.h"
#include "Sprite.h"

bool MainMenuScene::first_load = true;

std::shared_ptr<Scene> MainMenuScene::Init() {
	Scene* scene = new Scene(this);

    //qc = new QuickCamera();
    //qc->_sr(0.75f);
	//qc->_sm(100.0f);

    Camera::main->cameraTransform->position = glm::vec3(-9.96294f, -9.06299f, 2.38608f);
    Camera::main->cameraTransform->rotation = glm::vec2(47.0f, -4.0f);

    objs.push_back(new GameObject());
    am = new AnimationManager();
    objs[objs.size()-1]->AddScript(am);
    
    objs.push_back(new GameObject("BaseCube", glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f), glm::vec3(1000.0f, 1000.0f, 0.0f)));
    objs[objs.size()-1]->AddColorChange(glm::vec3(1.0f), glm::vec3(0.1f, 0.5f, 0.1f));
    
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
    
    if (first_load) {
        first_animation();
        first_load = false;
    }

	return std::shared_ptr<Scene>(scene);
}

void MainMenuScene::Update() {
    //qc->HandleMove();
    //qc->HandleRotate();
    if (!menu_options.empty())
        UpdateMenu();
    if (!maps_options.empty())
        UpdateMaps();

    if (Input::getKeyClicked(GLFW_KEY_B)) {
		std::cout << Camera::main->cameraTransform->position.x << ' ' << Camera::main->cameraTransform->position.y << ' ' << Camera::main->cameraTransform->position.z << '\n' << Camera::main->cameraTransform->rotation.x << ' ' << Camera::main->cameraTransform->rotation.y << '\n';
	}
}

void MainMenuScene::UnLoad() {}

void MainMenuScene::ShowMenu() {
    menu_choosen_option = 0;

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,0.39f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Play");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,0.4f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Play");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,0.09f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Appearance");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,0.1f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Appearance");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,-0.21f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Settings");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,-0.2f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Settings");

    menu_options.push_back({nullptr, nullptr});
    menu_options[menu_options.size()-1].second = (new Text("SansSerif", {0.208f,-0.51f,0.9f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].second->SetText("Exit");
    menu_options[menu_options.size()-1].second->SetColor(glm::vec4(glm::vec3(0.0f), 1.0f));
    menu_options[menu_options.size()-1].first = (new Text("SansSerif", {0.2f,-0.5f,0.8f}, {-1,-1}, 0.4f, glm::vec4(1.0f), -2.0f));
    menu_options[menu_options.size()-1].first->SetText("Exit");
}

void MainMenuScene::HideMenu() {
    for (auto& x : menu_options) {
        delete x.first;
        delete x.second;
    }
    menu_options.clear();
}

void MainMenuScene::UpdateMenu() {
    if (Input::getKeyClicked(GLFW_KEY_DOWN)) ++menu_choosen_option;
    if (Input::getKeyClicked(GLFW_KEY_UP))   --menu_choosen_option;

    menu_choosen_option = glm::normalize(menu_choosen_option, static_cast<int>(menu_options.size()));
    UpdateMenuHighlight();

    if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
        switch (menu_choosen_option)
        {
        case 0:
            to_maps_animation();
            break;
        case 1:
            
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
    if (Input::getKeyClicked(GLFW_KEY_ESCAPE))
        from_maps_animation();

    if (Input::getKeyClicked(GLFW_KEY_LEFT)) ++menu_choosen_option;
    if (Input::getKeyClicked(GLFW_KEY_RIGHT)) --menu_choosen_option;

    menu_choosen_option = glm::normalize(menu_choosen_option, static_cast<int>(maps_options.size()));
    UpdateMapsHighlight();

    if (Input::getKeyClicked(GLFW_KEY_ENTER)) {
        switch (menu_choosen_option)
        {
        case 0:
            Application::LoadScene("map_demo");
            break;
        case 1:    
            Application::LoadScene("map_demo");
            break;
        case 2:
            Application::LoadScene("map_demo");    
            break;
        }
    }
}

void MainMenuScene::UpdateMapsHighlight() {

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
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, {glm::vec3(-5.16202f, 1.9114f, 1.5676f), glm::vec2(24.5f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}, false, [&](){
        HideMenu();
    }});
    am->addToQueue({{glm::vec3(-5.16202f, 1.9114f, 1.5676f), glm::vec2(24.5f, -2.0f)}, {glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
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
    am->addToQueue({{glm::vec3(6.91982f, 5.95392f, 2.12385f), glm::vec2(11.0f, -2.0f)}, {glm::vec3(-5.16202f, 1.9114f, 1.5676f), glm::vec2(24.5f, -2.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-5.16202f, 1.9114f, 1.5676f), glm::vec2(24.5f, -2.0f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 1.0f, {0.0f, 0.0f, 0.0f}});
    am->addToQueue({{glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, {glm::vec3(-9.96294f, -9.06299f, 2.38608f), glm::vec2(47.0f, -4.0f)}, 0.01f, {0.0f, 0.0f, 0.0f}, true, [](){}, [&](){
        ShowMenu();
    }});
}