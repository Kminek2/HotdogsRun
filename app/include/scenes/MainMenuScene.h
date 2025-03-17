#pragma once
#include "SceeneScript.h"
#include "QuickCamera.h"
#include <utility>
#include <vector>
class MainMenuScene :
    public SceeneScript
{
private:
    QuickCamera* qc;
    std::vector<GameObject*> objs;
    static bool first_load;
    std::vector<std::pair<Text*, Text*>> menu_options;
    int menu_choosen_option;
    void ShowMenu();
    void UpdateMenu();
    void UpdateMenuHighlight();
public:
    static std::vector<std::pair<std::string, SceeneScript*>> scenes;
    
    std::shared_ptr<Scene> Init() override;
    
    void Update() override;
    
    void UnLoad() override;
};