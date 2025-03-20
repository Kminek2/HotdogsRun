#pragma once
#include "SceeneScript.h"
#include "objects/AnimationManager.h"
#include "QuickCamera.h"
#include <utility>
#include <vector>
class MainMenuScene :
    public SceeneScript
{
private:
    //QuickCamera* qc;
    AnimationManager* am;
    std::vector<GameObject*> objs;
    static bool first_load;
    std::vector<std::pair<Text*, Text*>> menu_options;
    std::vector<std::pair<Sprite*, std::pair<Text*, Text*>>> maps_options;
    int menu_choosen_option;
    void ShowMenu();
    void HideMenu();
    void UpdateMenu();
    void UpdateMenuHighlight();
    void first_animation();

    void ShowMaps();
    void UpdateMaps();
    void UpdateMapsHighlight();
    void HideMaps();

    void to_maps_animation();
    void from_maps_animation();
public:
    static std::vector<std::pair<std::string, SceeneScript*>> scenes;
    
    std::shared_ptr<Scene> Init() override;
    
    void Update() override;
    
    void UnLoad() override;
};